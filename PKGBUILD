# Maintainer: Sung Pae <self@sungpae.com>
pkgname=powerctl
pkgver=0
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
    mkdir -p "pkg/$pkgname/usr/bin"
    go build -i -v -o "pkg/$pkgname/usr/bin/$pkgname"
}
