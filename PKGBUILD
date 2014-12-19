# Maintainer: Sung Pae <self@sungpae.com>
pkgname=powerctl
pkgver=
pkgrel=1
pkgdesc="Power management in a single static binary."
license=('MIT')
arch=('x86_64')
groups=('nerv')
depends=('systemd')

pkgver() {
    git describe --long --tags | tr - .
}

package() {
    cd "$startdir"
    make CFLAGS="$CFLAGS -static" PREFIX='/usr/bin' DESTDIR="$pkgdir/" install
}
