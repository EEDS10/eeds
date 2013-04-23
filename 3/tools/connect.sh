#!/bin/bash

# uses sshpass because deal with it
sshpass -p 'perper' ssh -o 'StrictHostKeyChecking no' root@$(curl -s http://arkt.is/ip/)
