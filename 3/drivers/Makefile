DESTINATION = $$(wget -qO- http://arkt.is/ip/ || curl -s http://arkt.is/ip/)
SSHOPTS := 'StrictHostKeyChecking no'
SSHPASS := ../tools/sshpass -p 'perper'

all:
	cd buttons && make
	cd leds && make

upload:
	cd buttons && make upload
	cd leds && make upload
	$(SSHPASS) scp -o $(SSHOPTS) load_drivers.sh root@${DESTINATION}:


clean:
	cd buttons && make clean
	cd leds && make clean
