N="
"

_C=0
NO_EXPORT=1
LOAD_STATE=1
LIST_SEP=" "

config_cb() { return 0; }
option_cb() { return 0; }
list_cb() { return 0; }

append() {
	local var="$1"
	local value="$2"
	local sep="${3:- }"
	
	eval "export ${NO_EXPORT:+-n} -- \"$var=\${$var:+\${$var}\${value:+\$sep}}\$value\""
}
	
option () {	
	local varname="$1"; shift
	local value="$*"

	export ${NO_EXPORT:+-n} "CONFIG_${CONFIG_SECTION}_${varname}=$value"
	[ -n "$NO_CALLBACK" ] || option_cb "$varname" "$*"
}

# config_get <variable> <section> <option> [<default>]
# config_get <section> <option>
config_get() {
	case "$3" in
		"") eval echo "\${CONFIG_${1}_${2}:-\${4}}";;
		*)  eval export ${NO_EXPORT:+-n} -- "${1}=\${CONFIG_${2}_${3}:-\${4}}";;
	esac
}

config () {
	local cfgtype="$1"
	local name="$2"	
		
	export ${NO_EXPORT:+-n} CONFIG_NUM_SECTIONS=$(($CONFIG_NUM_SECTIONS + 1))
	name="${name:-cfg$CONFIG_NUM_SECTIONS}"
	append CONFIG_SECTIONS "$name"
	[ -n "$NO_CALLBACK" ] || config_cb "$cfgtype" "$name"
	export ${NO_EXPORT:+-n} CONFIG_SECTION="$name"
	export ${NO_EXPORT:+-n} "CONFIG_${CONFIG_SECTION}_TYPE=$cfgtype"
}

package() {
	return 0
}

export ${NO_EXPORT:+-n} CONFIG_SECTIONS=
export ${NO_EXPORT:+-n} CONFIG_NUM_SECTIONS=0

#DATA="$(../bin/test_shell_eval)"
DATA="package 'network'
config 'interface' 'loopback'
option 'ifname' 'lo'
option 'proto' 'static'
"
eval "$DATA"

echo "num_sections : $CONFIG_NUM_SECTIONS"
echo "sections : $CONFIG_SECTIONS"
echo "$CONFIG_loopback_TYPE"
config_get loopback ifname
config_get loopback proto

echo "=== DATA ==="
echo -e "$DATA"
echo "============"
unset DATA
