%define debug_package %{nil}

Name: cyan
Version: 2.0.0
Release: 1%{?dist}
Summary: Cyan Image Editor

License: LGPL-2.1
URL: https://cyan.fxarena.net
Source0: %{name}-%{version}.tar.gz

BuildRequires: gcc-c++ autoconf libpng-devel libjpeg-turbo-devel lcms2-devel libwebp-devel pango-devel cairo-devel libtiff-devel xz-devel zlib-devel fontconfig-devel qt5-qtbase-devel cmake3

%description
Simple general-purpose image editor

%prep
%setup -q

%build
STATIC=1 sh share/scripts/build-magick.sh
mkdir build && cd build
CYAN_VERSION=%{version} PKG_CONFIG_PATH=`pwd`/../ImageMagick/install/lib/pkgconfig cmake3 -DLINUX_DEPLOY=ON -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DMAGICK_PKG_CONFIG=Magick++-7.Q16HDRI ..
make 

%install
cd build
make DESTDIR=%{buildroot} install

%files
%defattr(-,root,root,-)
/usr/bin/Cyan
/usr/share/Cyan/profiles/*
/usr/share/applications/Cyan.desktop
/usr/share/doc/*
/usr/share/icons/hicolor/*
/usr/share/mime/packages/miff.xml

#%doc

%changelog

