#!/bin/sh
# 
# 1). ./test_for.sh 1 2 3
# 	$@ = "1" "2" "3"
# 	$* = "1 2 3"
#	$# = 3
# 2). ./test_for.sh 1/2 2 3
#	for file
#	相当于
#   for file in "$@"
#
#	set fnord `echo ":$file" | sed -ne 's/^:\//#/;s/^://;s/\// /g;s/^#/\//;p'`
#	shift
#	pathcomp=
#   for d
#	相当于
#   pathcomp=
#   for d in `echo ":$file" | sed -ne 's/^:\//#/;s/^://;s/\// /g;s/^#/\//;p'`
#    
echo "arg num : $#"
echo -e "\narg \$@ : $@"
for arg in "$@"; do
	echo $arg
done

echo -e "\narg \$* : $*"
for arg in "$*"; do
	echo $arg
done

echo -e "\nomit in *"
for arg
do
	echo $arg
done

# mkinstalldirs
echo -e "\ntest make directory hierarchy"
errstatus=0
for file
do
   set fnord `echo ":$file" | sed -ne 's/^:\//#/;s/^://;s/\// /g;s/^#/\//;p'`
   #shift first arg 'fnord'
   shift

   echo $@
   pathcomp=
   for d in ${1+"$@"} ; do
     pathcomp="$pathcomp$d"
     case "$pathcomp" in
       -* ) pathcomp=./$pathcomp ;;
     esac
	
     if test ! -d "$pathcomp"; then
        echo "mkdir $pathcomp" 1>&2
        #mkdir "$pathcomp" || errstatus=$?
     fi

     pathcomp="$pathcomp/"
   done
done

exit $errstatus

