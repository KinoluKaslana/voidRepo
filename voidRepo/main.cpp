#include <limits>
#include <type_traits>
template<typename _Ty>
struct _number_limits_integral {
private:
	template<typename _T>
	static constexpr  _T _max(char(*)[std::is_unsigned_v<_T>] = nullptr) {
		_T value = 0;
		constexpr  unsigned short size = sizeof(_T);
		for (unsigned int i = 0; i != size; i++) {
			value <<= 8;
			value |= 0xFF;
		}
		return value;
	}
	template<typename _T>
	static constexpr  _T _max(char(*)[!std::is_unsigned_v<_T>] = nullptr) {
		_T value = 0x7F;
		constexpr unsigned short size = sizeof(_T);
		for (unsigned int i = 0; i != size - 1; i++) {
			value <<= 8;
			value |= 0xFF;
		}
		return value;
	}
	template<typename _T>
	static constexpr  _T _min(char(*)[std::is_unsigned_v<_T>] = nullptr) {
		return 0;
	}
	template<typename _T>
	static constexpr  _T _min(char(*)[!std::is_unsigned_v<_T>] = nullptr) {
		_T value = 0x80;
		auto shift = (8 * (sizeof(_T) - 1));
		if (shift) value <<= shift;
		return value;
	}
public:
	static constexpr  _Ty max() {
		return _max<_Ty>();
	}
	static constexpr  _Ty min() {
		return _min<_Ty>();
	}
};
template<typename _Ty>
struct _number_limits_floating {

};

template<typename  _Ty = int>
struct number_limits :
	public  std::enable_if<std::is_arithmetic_v<_Ty>, typename std::conditional_t<
	std::is_integral_v<_Ty>,
	typename ::_number_limits_integral<_Ty>,
	typename ::_number_limits_floating<_Ty>>>::type{};

int main() {
	char ed = number_limits<char>::max();
	char edd = number_limits<char>::min();
	char edde = std::numeric_limits<char>::min();
	double ff = std::numeric_limits<double>::max();
	auto pff = &ff;
	return 0;
}

