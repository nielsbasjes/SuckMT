all:
	aclocal
	autoheader
	automake
	autoconf
	./configure
	${MAKE}
    
