To generate new header files, do the following steps:
#Remove current files:
rm -r ies/*.*
#Retrieve asn1c specification 
./scripts/update_3gpp_spec_ts38413.py -v 16.0.0 -o asn.1 38413
#Generate header files from .asn1 files
 ./scripts/generate_src_from_asn.1.sh
