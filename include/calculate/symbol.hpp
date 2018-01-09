#ifndef __CALCULATE_SYMBOL_HPP__
#define __CALCULATE_SYMBOL_HPP__

#include "wrapper.hpp"


namespace calculate {

template<typename Type, typename Expression>
class Symbol {
    using WrapperConcept = WrapperConcept<Type, Expression>;
    using Wrapper = Wrapper<Type, Expression>;

    friend struct std::hash<Symbol>;

public:
    enum class SymbolType : int {
        LEFT=0,
        RIGHT,
        SEPARATOR,
        CONSTANT,
        FUNCTION,
        OPERATOR
    };

private:
    template<typename Callable>
    struct Inspect {
        static constexpr bool not_me =
            detail::NotSame<Callable, Symbol>::value;
        static constexpr bool is_model =
            std::is_base_of<WrapperConcept, Callable>::value;
    };

    Wrapper _wrapper;

protected:
    virtual bool _equal(const Symbol&) const noexcept { return true; }

    std::size_t _hash() const noexcept {
        if (symbol() == SymbolType::CONSTANT)
            return std::hash<Type>()(_wrapper());
        return std::hash<Wrapper>()(_wrapper);
    }

public:
    template<
        typename Callable,
        std::enable_if_t<Inspect<Callable>::not_me>* = nullptr,
        std::enable_if_t<!Inspect<Callable>::is_model>* = nullptr
    >
    Symbol(Callable&& callable) :
            _wrapper{std::forward<Callable>(callable), &Expression::evaluate}
    {}

    template<
        typename Callable,
        std::enable_if_t<Inspect<Callable>::is_model>* = nullptr
    >
    Symbol(Callable&& callable) :
            _wrapper{std::forward<Callable>(callable)}
    {}

    bool operator==(const Symbol& other) const noexcept {
        if (symbol() != other.symbol())
            return false;

        if (symbol() == SymbolType::CONSTANT)
            return _wrapper() == other._wrapper();
        return _wrapper == other._wrapper && this->_equal(other);
    }

    Type evaluate(const std::vector<Expression>& nodes) const {
        return const_cast<const Wrapper*>(&_wrapper)->operator()(nodes);
    }

    Type evaluate(const std::vector<Expression>& nodes) {
        return _wrapper(nodes);
    }

    std::size_t arguments() const noexcept { return _wrapper.argc(); }

    virtual SymbolType symbol() const noexcept = 0;

    virtual ~Symbol() {}
};


template<typename Type, typename Expression>
class Constant final : public Symbol<Type, Expression> {
    using Symbol = Symbol<Type, Expression>;
    using SymbolType = typename Symbol::SymbolType;

public:
    Constant(Type value) :
            Symbol{[value]() noexcept { return value; }}
    {}

    SymbolType symbol() const noexcept override { return SymbolType::CONSTANT; }
};

template<typename Type, typename Expression>
class Variable final : public Symbol<Type, Expression> {
    using Symbol = Symbol<Type, Expression>;
    using SymbolType = typename Symbol::SymbolType;

public:
    Variable(Type& variable) :
            Symbol{[&variable]() noexcept { return variable; }}
    {}

    SymbolType symbol() const noexcept override { return SymbolType::CONSTANT; }
};

template<typename Type, typename Expression>
class Function final : public Symbol<Type, Expression> {
    using Symbol = Symbol<Type, Expression>;
    using SymbolType = typename Symbol::SymbolType;

public:
    template<typename Callable>
    Function(Callable&& callable) :
            Symbol{std::forward<Callable>(callable)}
    {}

    SymbolType symbol() const noexcept override { return SymbolType::FUNCTION; }
};

template<typename Type, typename Expression>
class Operator final : public Symbol<Type, Expression> {
    using Symbol = Symbol<Type, Expression>;
    using SymbolType = typename Symbol::SymbolType;

public:
    enum class Associativity : int {LEFT=0, RIGHT, BOTH};

private:
    std::string _alias;
    std::size_t _precedence;
    Associativity _associativity;

    bool _equal(const Symbol& other) const noexcept {
        return
            _alias == static_cast<Operator&>(other)._alias &&
            _precedence == static_cast<Operator&>(other)._precedence &&
            _associativity == static_cast<Operator&>(other)._associativity;
    }

public:
    template<typename Callable>
    Operator(
        Callable&& callable,
        const std::string& alias,
        std::size_t precedence,
        Associativity associativity
    ) :
            Symbol{std::forward<Callable>(callable)},
            _alias{alias},
            _precedence{precedence},
            _associativity{associativity}
    {}

    SymbolType symbol() const noexcept override { return SymbolType::OPERATOR; }

    const std::string& alias() const noexcept { return _alias; }

    std::size_t precedence() const noexcept { return _precedence; }

    Associativity associativity() const noexcept { return _associativity; }
};

}


namespace std {

template<typename Type, typename Expression>
struct hash<calculate::Symbol<Type, Expression>> {
    size_t operator()(const calculate::Symbol<Type, Expression>& symbol) const {
        return symbol._hash();
    }
};

}

#endif
