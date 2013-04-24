#!/bin/bash

# uses sshpass because deal with it
./sshpass -p 'perper' ssh -o 'StrictHostKeyChecking no' root@$(wget -qO- http://arkt.is/ip/ || curl -s http://arkt.is/ip/)
