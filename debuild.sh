# run debuild with .git ignore.
debuild -us -uc -I.git
git push --tags ssh://git.alioth.debian.org/git/collab-maint/binfmtc.git
git push  ssh://git.alioth.debian.org/git/collab-maint/binfmtc.git

