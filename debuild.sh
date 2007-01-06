# run debuild with .git ignore.
debuild -us -uc -i.git
git push --tags ssh://git.alioth.debian.org/git/collab-maint/binfmtc.git
git push --all ssh://git.alioth.debian.org/git/collab-maint/binfmtc.git

