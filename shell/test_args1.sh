#
echo "========== test_args1.sh =========="
##function test
#	$1 ->参数1，$2 ->参数2, ...
##
function test_printf()
{
	echo "$1 -> $2"
}
echo "The first call test_printf"
test_printf
echo "The second call test_printf"
test_printf "ok" "hello"
echo "==================================="
