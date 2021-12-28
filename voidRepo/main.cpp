#include <limits>
#include <type_traits>
template<typename _Ty>
struct _number_limits_integral {
private:
public:
	
};
template<typename _Ty>
struct _number_limits_floating {

};

template<typename  _Ty = int>
struct number_limits :
public  std::enable_if<std::is_arithmetic_v<_Ty>, std::conditional<
		std::is_integral_v<_Ty>,
		typename  _number_limits_integral<_Ty>,
		typename _number_limits_floating<_Ty>>::type>
::type{};
int main() {
	return 0;
}