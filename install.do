DESTDIR="${DESTDIR:-/usr}"

redo-ifchange ./cmus-rpcd
install -Dm0755 ./cmus-rpcd "${DESTDIR}/bin/cmus-rpcd"
