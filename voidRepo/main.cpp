#include <limits>
#include <vector>
#include <type_traits>
#include <algorithm>
using namespace std;

// simple imp. with intg.

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


int main() {

	vector<int> array = { 6, 4, 5, 2, 8 };
	MergeSort(array);

	return 0;
}

