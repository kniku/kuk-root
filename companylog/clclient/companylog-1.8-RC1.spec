Summary: A companylog client for linux
Name: companylog
Version: 1.3
Release: RC1
License: GPL
Group: Applications
URL: empty
Source0: %{name}-%{version}-%{release}.tar.gz
#BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
A Companyserv(c) client for Linux
%prep
%setup
%build

%install
mkdir /usr/local/companylog
#chmod a+rw /usr/local/companylog
cp *.png /usr/local/companylog
cp *.jpg /usr/local/companylog
cp *.so* /usr/local/companylog
cp companylog /usr/local/companylog
cp -r imageformats /usr/local/companylog/
#cp companylog.desktop /usr/share/applnk
cp companylog.desktop /usr/share/applications
cp companylog.png /usr/share/pixmaps
ln -s /usr/local/companylog/companylog /usr/bin/companylog
ln -s /usr/local/companylog/libQtGui.so.4.4.0 /lib/libQtGui.so.4
ln -s /usr/local/companylog/libQtCore.so.4.4.0 /lib/libQtCore.so.4
ln -s /usr/local/companylog/libQtNetwork.so.4.4.0 /lib/libQtNetwork.so.4
#ln -s /usr/local/companylog/libqjpeg.so /lib/libqjpeg.so
/sbin/ldconfig
#rm -rf $RPM_BUILD_ROOT
%clean
rm -rf $RPM_BUILD_ROOT


%changelog
* Fri Mar  14 2008 Martin Paar <martin.paar@gmx.at>
- Version 1.3-RC1

* Fri Oct  12 2007 Martin Paar <martin.paar@gmx.at>
- Version 1.2-RC1

#* Wed Mai  30 2007 Martin Paar <martin.paar@gmx.at>
#- Version 1.1-RC1

* Fri Oct  6 2006 Martin Paar <martin.paar@gmx.at>
- Version 1.0-RC1 Initial build

%files 
/usr/local/companylog/
/usr/bin/companylog
/lib/libQtGui.so.4
/lib/libQtCore.so.4
/lib/libQtNetwork.so.4
#/lib/libqjpeg.so
/usr/share/pixmaps/companylog.png
#/usr/share/applnk/companylog.desktop
/usr/share/applications/companylog.desktop
#%defattr(-,root,root,-)

