#include <vector>
#include <array>
#include <string>
#include <list>
#include <type_traits>
struct nBase {};

template<typename _Ty>
struct typeBase {
	using type = _Ty;
};

template<typename _Ty> struct removeClassMemberSpecifiers : typeBase<_Ty> {};

template<typename _Ty> struct removeClassMemberSpecifiers<_Ty const> : typeBase<_Ty> {};

template<typename ... Args>
using removeClassMemberSpecifiers_t = typename removeClassMemberSpecifiers<Args...>::type;

template<typename _Ty, typename ... >
struct removeClassMemberFunctionSpecifiers : typeBase<_Ty> {};

template<typename _Ty, typename cTy, typename ... Args>
struct removeClassMemberFunctionSpecifiers<_Ty(cTy::*)(Args...)> : typeBase<_Ty(cTy::*)(Args...)> {};

template<typename _Ty, typename cTy, typename ... Args>
struct removeClassMemberFunctionSpecifiers<_Ty(cTy::*) (Args...) const noexcept> : typeBase<_Ty(cTy::*)(Args...)> {};
template<typename ... Args>
using removeClassMemberFunctionSpecifiers_t = typename removeClassMemberFunctionSpecifiers<Args...>::type;

template<typename _Ty, typename ... Args>
struct funcType : typeBase<int> {};

template<typename _Ty, typename ... Args>
struct funcType<_Ty(Args ...)> : typeBase<_Ty(Args ...)> {};

template<typename _Ty, typename ... Args>
struct funcType<_Ty(*)(Args ...)> : typeBase<_Ty(Args ...)> {};

template<typename _Ty, typename cTy, typename ... Args>
struct funcType<_Ty(cTy::*) (Args ...)> : typeBase<_Ty(Args ...)> {};

template<typename _Ty>
using funcType_t = typename funcType<_Ty>::type;

template<typename _Ty>
constexpr bool isFunc(_Ty par) {
	using type = removeClassMemberFunctionSpecifiers_t<removeClassMemberSpecifiers_t<decltype(par)>>;
	return std::is_function_v<funcType_t<type>>;
}


template<typename _Ty>
struct typeHelper {
	using type = nBase;
};

template<>
struct typeHelper<void> {
	using type = void;
};

template<typename _Ty, typename = void >
struct isContainer : public std::false_type {};

template<typename _Ty>
struct isContainer<_Ty,
	std::void_t<
	typename _Ty::iterator,								//named_req/Container#Types 
	typename _Ty::value_type,
	typename _Ty::reference,
	typename _Ty::const_reference,
	typename _Ty::const_iterator
	>> : public std::true_type, typeHelper<_Ty>{};

template<typename _Ty>
constexpr bool isContainer_v = isContainer<_Ty>::value;

template<typename _Ty>
struct isPoint : std::false_type {};

template<typename _Ty>
struct isPoint<_Ty*> : std::true_type, typeHelper<_Ty> {};

template<typename _Ty>
constexpr bool isPoint_v = isPoint<_Ty>::value;

template<typename _Ty, typename classTy = void>
struct isMemberPoint : std::false_type {};

template<typename _Ty, typename classTy>
struct isMemberPoint<_Ty classTy::*> : std::true_type, typeHelper<_Ty> {
	using classType = classTy;
};

template<typename _Ty>
constexpr bool isMemberPoint_v = isMemberPoint<_Ty>::value;

template<typename _Ty, typename _ = void>
struct contigousContainerChecker : public std::false_type {};

template<typename _Ty>
struct contigousContainerChecker<_Ty, std::enable_if_t<isFunc(&_Ty::iterator::operator+)>> :public std::true_type {};

template<typename _Ty, typename _ = void>
struct isContigousContainerImpl : std::false_type {};

template<typename _Ty>
struct isContigousContainerImpl<_Ty, std::enable_if_t<isContainer_v<_Ty> && !isMemberPoint_v<typename _Ty::iterator> && isPoint_v<typename _Ty::iterator>>> :
	std::true_type, typeHelper<_Ty>
{};

template<typename _Ty>
struct isContigousContainerImpl<_Ty, std::enable_if_t<isContainer_v<_Ty>&& contigousContainerChecker<_Ty>::value>> :
	std::true_type, typeHelper<_Ty>
{};

template<typename ...>
struct isContiguousContainer : isContigousContainerImpl<void> {};

template<typename _Ty>
struct isContiguousContainer<_Ty> : isContigousContainerImpl<_Ty> {};

template<typename ...>
constexpr bool isContigousContainerHelper(...) {
	return false;
}

template<typename _Ty>
constexpr bool isContigousContainerHelper(decltype(&_Ty::iterator::operator+)) {
	return isContiguousContainer<_Ty>::value;
}

template<typename _Ty>
constexpr bool isContiguousContainer_v = isContiguousContainer<_Ty>::value;

void f() {
	auto e1 = isContiguousContainer_v<std::vector<int>>;
	auto e2 = isContiguousContainer_v<std::vector<int>>;
	auto e3 = isContiguousContainer_v<std::vector<bool>>;
	auto e4 = isContiguousContainer_v<std::list<int>>;
	auto e5 = isContiguousContainer_v<std::array<int, 6>>;
}

int main() {
	f();
	return 0;
}

