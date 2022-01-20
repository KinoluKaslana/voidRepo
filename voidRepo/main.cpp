#include <limits>
#include <vector>
#include <type_traits>
#include <algorithm>
using namespace std;

struct nBase {};

template<typename _Ty>
struct typeBase {
	using type = _Ty;
};

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
	return std::is_function<typename funcType<decltype(par)>::type>::value;
}


template<typename _Ty>
struct typeHelper {
	using type = nBase;
};

template<>
struct typeHelper<void> {
	using type = void;
};
// simple imp. with intg.
template<typename _Ty, typename = void >
struct isContainer : public std::false_type, typeHelper<void> {};

template<typename _Ty>
struct isContainer<_Ty,
	std::void_t<
	typename _Ty::iterator,								//named_req/Container#Types 
	typename _Ty::value_type,
	typename _Ty::reference,
	typename _Ty::const_reference,
	typename _Ty::const_iterator
	>> : public std::true_type, typeHelper<_Ty> {};

template<typename _Ty,typename bTy = void>
struct contigousContainerChecker : public std::false_type, typeHelper<void> {};

template<typename _Ty>
struct contigousContainerChecker<_Ty,
	std::enable_if<isFunc(&_Ty::iterator::operator+),_Ty>
	> :public std::true_type, typeHelper<_Ty> {};

template<typename _Ty, typename = void>
struct isContigousContainerImpl : std::false_type, typeHelper<void> {};

template<typename _Ty>
struct isContigousContainerImpl<_Ty, int>:
	std::enable_if<isContainer<_Ty>::value && contigousContainerChecker<_Ty>::value, std::true_type>::type, typeHelper<_Ty>
{};
template<typename _Ty>
struct isContiguousContainer : isContigousContainerImpl<_Ty> {};

template<typename _Ty, typename _TySize>
void MergeSortImpl(vector<_Ty>& ds,const _TySize low,const _TySize mid,const _TySize max) {
	vector<_Ty> tDs(max - low + 1);
	copy(ds.begin() + low, ds.begin() + max + 1, tDs.begin());
	auto lowIndex = 0;
	auto highIndex = mid - low;
	auto maxIndex = max - low;
	auto midIndex = mid - low;
	auto index = low;

	while(lowIndex < midIndex && highIndex < maxIndex) {
		if(tDs[highIndex] < tDs[lowIndex]) {
			ds[index++] = tDs[highIndex++];
		}
		else if(tDs[highIndex] > tDs[lowIndex]) {
			ds[index++] = tDs[lowIndex++];
		}
	}
	while(lowIndex < midIndex) {
		if(index <= max) {
			ds[index++] = tDs[lowIndex++];
		}
		else {
			throw "overflow";
		}
	}
	while (highIndex < maxIndex) {
		if (index <= max) {
			ds[index++] = tDs[highIndex++];
		}
		else {
			throw "overflow";
		}
	}
}



template<typename _Ty, typename _TySize>
void MergeSort(vector<_Ty>& ds,const _TySize low,const _TySize max) {
	if ((max - low) <= 1) return;
	auto mid = low + (max - low + 1) / 2;
	MergeSort(ds, low, mid);
	MergeSort(ds, mid, max);
	MergeSortImpl(ds, low, mid, max);
}
template<typename _Ty>
void MergeSort(vector<_Ty>& ds) {
	auto max = ds.size() - 1;
	MergeSort(ds, static_cast<decltype(max)>(0), max);
}


void f() {
	auto e1 = isContainer<std::vector<int>>::value;
	std::remove_const_t<decltype(&std::vector<int>::iterator::operator+)> eq;
	isFunc(&std::vector<int>::iterator::operator+);
}

int main() {

	vector<int> array = { 6, 4, 5, 2, 8 };
	MergeSort(array);
	return 0;
}

