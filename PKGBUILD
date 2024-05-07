# Maintainer: l6174 <l6174@proton.me>

pkgname=todominal
pkgver=2.0
pkgrel=1
pkgdesc="A todo list application for the terminal"
arch=('any')
url="https://github.com/l6174/todominal"
OPTIONS=(strip !debug)
license=('GPL')
depends=('figlet' 'gcc')

source=("git+https://github.com/l6174/todominal.git")

build() {
  cd "$srcdir/$pkgname"
  g++ main.cpp -o todominal
}

package() {
  cd "$srcdir/$pkgname"
  
  # Install application binary
  install -Dm755 todominal "$pkgdir/usr/bin/todominal"
  install -Dm755 todominal-rofi "$pkgdir/usr/bin/todominal-rofi"
  
  # Install license file
  install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
  
  # Install README file (optional)
  install -Dm644 README.md "$pkgdir/usr/share/doc/$pkgname/README.md"
}
sha256sums=('SKIP')
