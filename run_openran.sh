# have to run all the functions, then it can work without error
./install/bin/open5gs-nrfd &
./install/bin/open5gs-scpd &
./install/bin/open5gs-amfd &

# # this let the network know the subscriber
./install/bin/open5gs-pcfd &
./install/bin/open5gs-smfd &

./install/bin/open5gs-upfd &
./install/bin/open5gs-ausfd &
./install/bin/open5gs-udmd &
./install/bin/open5gs-udrd &
./install/bin/open5gs-mmed &

./install/bin/open5gs-hssd &
./install/bin/open5gs-nssfd &
./install/bin/open5gs-bsfd &
./install/bin/open5gs-pcrfd &
./install/bin/open5gs-sgwud &
./install/bin/open5gs-sgwcd &
