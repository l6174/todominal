# Maintainer: l6174 <l6174@proton.me>

pkgname=todominal
pkgver=2.2
pkgrel=3
pkgdesc="A todo list application for the terminal and rofi"
arch=('any')
url="https://github.com/l6174/todominal"
OPTIONS=(strip !debug)
license=('GPL')
depends=('figlet' 'sed')
makedepends=('gcc')
optdepends=('rofi: rofi support')

source=("git+https://github.com/l6174/todominal.git")

build() {
  cd "$srcdir/$pkgname/src"
  g++ main.cpp -o "$srcdir/$pkgname/bin/todominal"
}

package() {
  # Create necessary directories
  install -d "$pkgdir/usr/bin"
  install -d "$pkgdir/usr/share/licenses/$pkgname"
  install -d "$pkgdir/usr/share/doc/$pkgname"

  # Install application binary
  install -m755 "$srcdir/$pkgname/bin/todominal" "$pkgdir/usr/bin/"

  # Install rofi binary
  install -m755 "$srcdir/$pkgname/bin/todominal-rofi" "$pkgdir/usr/bin/"

  # Install license file
  install -m644 "$srcdir/$pkgname/LICENSE" "$pkgdir/usr/share/licenses/$pkgname/"

  # Install README file
  install -m644 "$srcdir/$pkgname/README.md" "$pkgdir/usr/share/doc/$pkgname/"
}
sha256sums=('SKIP')
