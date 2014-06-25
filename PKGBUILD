# Maintainer: Sung Pae <self@sungpae.com>
pkgname=powerctl
pkgver=v000.2.gd808274
pkgrel=1
pkgdesc="Power management in a single static binary."
arch=('x86_64')
groups=('guns')
depends=('systemd')

pkgver() {
    printf %s "$(git describe --long --tags | tr - .)"
}

package() {
    cd "$startdir"
    make CFLAGS="$CFLAGS -static" PREFIX='/usr/bin' DESTDIR="$pkgdir/" install
}
