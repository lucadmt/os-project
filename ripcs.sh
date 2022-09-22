for q in `ipcs -q | egrep ^q | awk '{ print $2; }'`; do ipcrm -q $q; done
for m in `ipcs -m | egrep ^m | awk '{ print $2; }'`; do ipcrm -m $m; done
for s in `ipcs -s | egrep ^s | awk '{ print $2; }'`; do ipcrm -s $s; done
