default: qvm
qvm: build_qvm
so: build_so

ZIP = zip

build_qvm:
	$(MAKE) -C Code-DM build_qvm
build_so:
	$(MAKE) -C Code-DM build_so

clean:
	$(MAKE) -C Code-DM clean
	rm -f baseef/*.so baseef/*.pk3 baseef/vm/*.qvm

pak: qvm
	cd baseef && rm -f pak4.pk3 && $(ZIP) -r pak4.pk3 vm/*
default: build

build_qvm:
	$(MAKE) -C game build_qvm
	$(MAKE) -C cgame build_qvm
	$(MAKE) -C ui build_qvm

build_so:
	$(MAKE) -C game build_so
	$(MAKE) -C cgame build_so
	$(MAKE) -C ui build_so

clean:
	$(MAKE) -C game clean
	$(MAKE) -C cgame clean
	$(MAKE) -C ui clean
default: qvm
qvm: build_qvm
so: build_so

ZIP = zip

build_qvm:
	$(MAKE) -C Code-DM build_qvm
build_so:
	$(MAKE) -C Code-DM build_so

clean:
	$(MAKE) -C Code-DM clean
	rm -f baseef/*.so baseef/*.pk3 baseef/vm/*.qvm

pak: qvm
	cd baseef && rm -f pak4.pk3 && $(ZIP) -r pak4.pk3 vm/*
