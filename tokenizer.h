//
// Created by andrey on 6/26/15.
//

#ifndef REVERSEPOLISHNOTATION_TOKENIZER_H
#define REVERSEPOLISHNOTATION_TOKENIZER_H

#include <string>
#include <algorithm>
#include <type_traits>

enum operator_type {
    OPERATOR_UNKNOWN = 0,
    OPERATOR_OPENED_BRACKET,
    OPERATOR_CLOSED_BRACKET,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLICATION,
    OPERATOR_DIVISION
};

template<typename T>
class token {
public:
    enum type {
        UNKNOWN = 0,
        OPERAND,
        OPERATOR,
        END
    };

    token(T v) {
        setValue(v);
        setType(OPERAND);
        setOperator(OPERATOR_UNKNOWN);
    }

    token(operator_type o) {
        setOperator(o);
        setValue(T());
        setType(OPERATOR);
    }

    token(type t) {
        setValue(T());
        setType(t);
        setOperator(OPERATOR_UNKNOWN);
    }

    T getValue() const { return value_; }
    void setValue(T value_) { token::value_ = value_; }
    type getType() const { return  type_; }
    void setType(type t) { type_ = t; }
    operator_type getOperator() { return operator_type_; }
    void setOperator(operator_type o) { operator_type_ = o; }
    int getOperatorPriority() const {
        if (type_ != OPERATOR)
            return -1;
        else {
            switch (operator_type_) {
                case OPERATOR_UNKNOWN:
                    return -1;
                case OPERATOR_OPENED_BRACKET:
                case OPERATOR_CLOSED_BRACKET:
                    return 0;
                case OPERATOR_PLUS:
                case OPERATOR_MINUS:
                    return 1;
                case OPERATOR_MULTIPLICATION:
                case OPERATOR_DIVISION:
                    return 2;
            }
        }
        return -1;
    }

private:
    T value_;
    type type_;
    operator_type operator_type_;
};

template<typename T>
class tokenizer {
public:
    typedef token<T> token_t;

    tokenizer() {}

    tokenizer(const std::string& s) {
        setExpression(s);
    }

    void setExpression(const std::string& s) {
        expression_ = s;
        iterator_ = expression_.begin();
    }

    bool parse(std::string s) {
        auto iterEnd = std::remove_copy_if(s.begin(), s.end(), s.begin(), [](const std::string::value_type& v) {
            bool b = v == ' ';
            return b;
        });
        s.erase(iterEnd);
        return false;
    }

    token_t getNextToken() {
        if (iterator_ == expression_.end())
            return token_t(token_t::END); // end
        while (*iterator_ == ' ')
            ++iterator_;
        std::string sOperand;
        while (*iterator_ >= '0' && *iterator_ <= '9' ||
               *iterator_ == '.' || *iterator_ == ',') {
            sOperand += *iterator_++;
        }
        if (!sOperand.empty()) {
            T dOperand = std::stod(sOperand);
            return token_t(dOperand);
        }
        else if (*iterator_ == '+') {
            ++iterator_;
            return token_t(OPERATOR_PLUS);
        } else if (*iterator_ == '-') {
            ++iterator_;
            return token_t(OPERATOR_MINUS);
        } else if (*iterator_ == '*') {
            ++iterator_;
            return token_t(OPERATOR_MULTIPLICATION);
        } else if (*iterator_ == '/') {
            ++iterator_;
            return token_t(OPERATOR_DIVISION);
        } else if (*iterator_ == '(') {
            ++iterator_;
            return token_t(OPERATOR_OPENED_BRACKET);
        } else if (*iterator_ == ')') {
            ++iterator_;
            return token_t(OPERATOR_CLOSED_BRACKET);
        }

        return token_t(token_t::UNKNOWN);
    }

private:
    std::string::iterator iterator_;
    std::string expression_;
};


#endif //REVERSEPOLISHNOTATION_TOKENIZER_H
