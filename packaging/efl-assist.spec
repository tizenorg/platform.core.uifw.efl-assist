%bcond_with x

Name:       efl-assist
Summary:    EFL assist library
Version:    0.1.18r02
Release:    0
Group:      System/Libraries
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(elementary)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(tts)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(capi-appfw-application)
%if %{with x}
BuildRequires:  pkgconfig(ecore-x)
%endif

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig


%description
EFL assist library


%package devel
Summary:    EFL assist library (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   capi-base-common-devel


%description devel
EFL assist library providing small utility functions (devel)


%prep
%setup -q


%build
export CFLAGS+=" -fvisibility=hidden"
export LDFLAGS+=" -fvisibility=hidden"

%cmake . \
    -DCMAKE_INSTALL_PREFIX=/usr \
%if %{with x}
    -DX11_SUPPORT=On \
%endif
    #eol


make %{?jobs:-j%jobs}


%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/usr/share/license
cp %{_builddir}/%{buildsubdir}/LICENSE %{buildroot}/usr/share/license/%{name}


%post -p /sbin/ldconfig


%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
#%{_bindir}/*
%{_libdir}/libefl-assist.so.*
%manifest %{name}.manifest
%license /usr/share/license/%{name}


%files devel
%defattr(-,root,root,-)
%{_includedir}/efl-assist/*.h
%{_libdir}/*.so
%{_libdir}/pkgconfig/efl-assist.pc

