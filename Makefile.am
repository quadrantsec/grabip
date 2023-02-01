AUTOMAKE_OPIONS=foreign no-dependencies subdir-objects
ACLOCAL_AMFLAGS =
SUBDIRS = src

#INCLUDES = @INCLUDES@

install-data-local:
	test -z "$(DESTDIR)$(sysconfdir)" || /bin/mkdir -p "$(DESTDIR)$(sysconfdir)"
	test -z "$(DESTDIR)$(bindir)" || /bin/mkdir -p "$(DESTDIR)$(bindir)"
	$(INSTALL) -m 755 src/grabip "$(DESTDIR)$(bindir)/grabip"
	@echo ""
	@echo "------------------------------------------------------------------------------"
	@echo "GrabIP has been installed!"
	@echo "------------------------------------------------------------------------------"
	@echo ""



