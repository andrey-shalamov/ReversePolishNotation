//
// Created by andrey on 6/26/15.
//

#ifndef REVERSEPOLISHNOTATION_RPN_H
#define REVERSEPOLISHNOTATION_RPN_H

#include <stack>
#include <string>
#include <queue>
#include "tokenizer.h"

template<typename T>
class rpn {
public:
    struct error {
        error() {
            code_ = 0;
            what_ = "ok";
        }
        error(const std::string& what, int code = 1) {
            code_ = code;
            what_ = what;
        }
        int code_;
        std::string what_;

        bool operator ! () const {
            return code_ == 0;
        }
    };

    typedef token<T> token_t;

    error calculate(token_t& o_first_operand, const token_t& second_operand, operator_type operation_type_) {
        if (o_first_operand.getType() != token_t::OPERAND) {
            return error("first parameter isn't operand");
        } else if (second_operand.getType() != token_t::OPERAND) {
            return error("second parameter isn't operand");
        }
        switch (operation_type_) {
            case OPERATOR_PLUS: {
                T res = o_first_operand.getValue() + second_operand.getValue();
                o_first_operand.setValue(res);
            } break;
            case OPERATOR_MINUS: {
                T res = o_first_operand.getValue() - second_operand.getValue();
                o_first_operand.setValue(res);
            } break;
            case OPERATOR_MULTIPLICATION: {
                T res = o_first_operand.getValue() * second_operand.getValue();
                o_first_operand.setValue(res);
            } break;
            case OPERATOR_DIVISION: {
                if (second_operand.getValue() == 0) {
                    return error("division by zero");
                }
                T res = o_first_operand.getValue() / second_operand.getValue();
                o_first_operand.setValue(res);
            } break;
            default: {
                return error("unsupported operation");
            }
        }
        return error();
    }

    error calculate(const std::string& i_expression, T& o_result) {
        // parse expression to reverse polish notation
        tokenizer_.setExpression(i_expression);
        std::queue<token_t> rpn_tokens;
        std::stack<token_t> rpn_operators;
        token_t token_ = tokenizer_.getNextToken();
        while (token_.getType() != token_t::END) {
            if (token_.getType() == token_t::UNKNOWN)
                return error("parsing error");
            else if (token_.getType() == token_t::OPERAND) {
                rpn_tokens.push(token_);
            }
            else if (token_.getType() == token_t::OPERATOR) {
                if (token_.getOperator() == OPERATOR_OPENED_BRACKET)
                    rpn_operators.push(token_);
                else if (token_.getOperator() == OPERATOR_CLOSED_BRACKET) {
                    if (rpn_operators.empty())
                        return error("uncoordinated brackets");
                    while (rpn_operators.top().getOperator() != OPERATOR_OPENED_BRACKET) {
                        rpn_tokens.push(rpn_operators.top());
                        rpn_operators.pop();
                        if (rpn_operators.empty())
                            return error("uncoordinated brackets");
                    }
                    rpn_operators.pop(); // remove opened bracket
                }
                else {
                    if (rpn_operators.empty())
                        rpn_operators.push(token_);
                    else {
                        int iPriority = token_.getOperatorPriority();
                        token_t top_operator_from_stack = rpn_operators.top();
                        while (iPriority <= top_operator_from_stack.getOperatorPriority()) {
                            rpn_tokens.push(top_operator_from_stack);
                            rpn_operators.pop();
                            if (rpn_operators.empty())
                                break;
                            top_operator_from_stack = rpn_operators.top();
                        }
                        rpn_operators.push(token_);
                    }
                }
            }
            token_ = tokenizer_.getNextToken();
        }
        while (!rpn_operators.empty()) {
            rpn_tokens.push(rpn_operators.top());
            rpn_operators.pop();
        }

        // calculation of reverse polish notation expression
        std::stack<token_t> result_stack;
        while (!rpn_tokens.empty()) {
            token_t current_token = rpn_tokens.front();
            if (current_token.getType() == token_t::OPERAND) {
                result_stack.push(current_token);
                rpn_tokens.pop();
            } else if (current_token.getType() == token_t::OPERATOR) {
                if (result_stack.size() < 2) {
                    return error("expression is not correct");
                }
                token_t second_operand = result_stack.top();
                if (second_operand.getType() != token_t::OPERAND) {
                    return error("expression is not correct");
                }
                result_stack.pop();
                token_t first_operand = result_stack.top();
                if (first_operand.getType() != token_t::OPERAND) {
                    return error("expression is not correct");
                }
                result_stack.pop();

                error e = calculate(first_operand, second_operand, current_token.getOperator());
                if (!e) {
                    result_stack.push(first_operand);
                    rpn_tokens.pop();
                }
                else {
                    return e;
                }
            }
        }
        if (result_stack.size() == 1 && result_stack.top().getType() == token_t::OPERAND) {
            o_result = result_stack.top().getValue();
            return error();
        } else {
            return error("something wrong");
        }
    }
private:
    tokenizer<T> tokenizer_;
};


#endif //REVERSEPOLISHNOTATION_RPN_H
