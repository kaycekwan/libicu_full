dnl aclocal.m4 for ICU
dnl Stephen F. Booth

dnl @TOP@

dnl ICU_CHECK_MH_FRAG
AC_DEFUN(ICU_CHECK_MH_FRAG, [
	AC_CACHE_CHECK(
		[which Makefile fragment to use],
		[icu_cv_host_frag],
		[
case "${host}" in
*-*-solaris*) 	
	if test "$ac_cv_prog_gcc" = yes; then	
		icu_cv_host_frag=$srcdir/config/mh-solaris-gcc 
	else
		icu_cv_host_frag=$srcdir/config/mh-solaris 
	fi ;;
*-*-linux*) 	icu_cv_host_frag=$srcdir/config/mh-linux ;;
*-*-aix*) 	icu_cv_host_frag=$srcdir/config/mh-aix ;;
*-*-hpux*)
	case "$CXX" in 
	*aCC)    icu_cv_host_frag=$srcdir/config/mh-hpux-acc ;;
	*CC)     icu_cv_host_frag=$srcdir/config/mh-hpux-cc ;;
	esac;;
*-*-os390*)	icu_cv_host_frag=$srcdir/config/mh-os390 ;;
*) 		icu_cv_host_frag=$srcdir/config/mh-unknown ;;
esac
		]
	)
])

dnl ICU_CONDITIONAL - Taken from Automake 1.4
AC_DEFUN(ICU_CONDITIONAL,
[AC_SUBST($1_TRUE)
AC_SUBST($1_FALSE)
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi])
