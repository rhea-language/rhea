# Copyright (c) 2024 - Nathanne Isip
# This file is part of Rhea.
# 
# Rhea is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
# 
# Rhea is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Rhea. If not, see <https://www.gnu.org/licenses/>.

import os
import platform
import shutil
import subprocess

arch_map = {
    'x86_64': 'amd64',
    'aarch64': 'arm64',
    'armv7l': 'armhf',
    'i686': 'i386',
    'i386': 'i386',
}

machine_arch = platform.machine()

PROJECT_NAME    = 'rhea-lang'
VERSION         = '1.0.0'
MAINTAINER      = 'Nathanne Isip <nathanneisip@gmail.com>'
DESCRIPTION     = 'Rhea programming language runtime and modules.'
INSTALL_DIR     = f'opt/{PROJECT_NAME}'
DEB_DIR         = os.path.join('dist', 'deb')
ARCHITECTURE    = arch_map.get(machine_arch, 'all')
OUTPUT_DEB      = os.path.join('dist', f'{PROJECT_NAME}_{VERSION}_{ARCHITECTURE}.deb')

def create_directory(path, mode=0o755):
    os.makedirs(path, exist_ok=True)
    os.chmod(path, mode)

if os.path.exists(DEB_DIR):
    shutil.rmtree(DEB_DIR)

try:
    create_directory(os.path.join(DEB_DIR, 'DEBIAN'))
    create_directory(os.path.join(DEB_DIR, INSTALL_DIR))
    create_directory(os.path.join(DEB_DIR, 'etc/profile.d'))
    create_directory(os.path.join(DEB_DIR, 'etc/fish/conf.d'))

    shutil.copytree(
        os.path.join('dist', 'rhea-lang'),
        os.path.join(DEB_DIR, INSTALL_DIR),
        dirs_exist_ok=True
    )

except Exception as e:
    print(f'Failed to copy modules: {str(e)}')
    exit(1)

posix_script_content = f'''#!/bin/sh
export RHEA_PATH="/opt/{PROJECT_NAME}"
'''

with open(os.path.join(DEB_DIR, 'etc/profile.d/rhea-lang.sh'), 'w') as f:
    f.write(posix_script_content)
os.chmod(os.path.join(DEB_DIR, 'etc/profile.d/rhea-lang.sh'), 0o755)

fish_script_content = f'''#!/usr/bin/fish
set -gx RHEA_PATH /opt/{PROJECT_NAME}
'''

with open(os.path.join(DEB_DIR, 'etc/fish/conf.d/rhea-lang.fish'), 'w') as f:
    f.write(fish_script_content)
os.chmod(os.path.join(DEB_DIR, 'etc/fish/conf.d/rhea-lang.fish'), 0o755)

postinst_content = '''
#!/bin/sh
set -e
export RHEA_PATH="/opt/rhea-lang"

add_source_command() {
    local file="$1"
    local source_cmd="$2"

    if [ -f "$file" ]; then
        grep -q "$source_cmd" "$file" || {
            echo "" >> "$file"
            echo "$source_cmd" >> "$file"
        }
    fi
}

add_source_command "/etc/bash.bashrc" "source /etc/profile.d/rhea-lang.sh"
add_source_command "/etc/zsh/zshenv" "source /etc/profile.d/rhea-lang.sh"

if [ -f "/etc/profile" ]; then
    add_source_command "/etc/profile" "source /etc/profile.d/rhea-lang.sh"
fi

chmod 755 /etc/profile.d/rhea-lang.sh
chmod 755 /etc/fish/conf.d/rhea-lang.fish

ln -s /opt/rhea-lang/bin/rhea /usr/bin/rhea
chmod 755 /usr/bin/rhea

echo "Rhea Language has been installed successfully!"
exit 0
'''

postinst_path = os.path.join(DEB_DIR, 'DEBIAN/postinst')
with open(postinst_path, 'w') as postinst_file:
    postinst_file.write(postinst_content)
os.chmod(postinst_path, 0o755)

postrm_content = '''
#!/bin/sh
sed -i '/source \\/etc\\/profile.d\\/rhea-lang.sh/d' /etc/zsh/zshenv
sed -i '/source \\/etc\\/profile.d\\/rhea-lang.sh/d' /etc/bash.bashrc
sed -i '/source \\/etc\\/profile.d\\/rhea-lang.sh/d' /etc/profile

rm -f /usr/bin/rhea
rm -rf /opt/rhea-lang

exit 0
'''

postrm_path = os.path.join(DEB_DIR, 'DEBIAN/postrm')
with open(postrm_path, 'w') as postrm_file:
    postrm_file.write(postrm_content)
os.chmod(postrm_path, 0o755)

control_content = f'''Package: {PROJECT_NAME}
Version: {VERSION}
Architecture: {ARCHITECTURE}
Maintainer: {MAINTAINER}
Description: {DESCRIPTION}
Priority: optional
Section: development
'''

control_file_path = os.path.join(DEB_DIR, 'DEBIAN/control')
with open(control_file_path, 'w') as control_file:
    control_file.write(control_content)
os.chmod(control_file_path, 0o644)
os.chmod(os.path.join(DEB_DIR, 'DEBIAN'), 0o755)

try:
    subprocess.run(['dpkg-deb', '-Z', 'xz', '--build', DEB_DIR, OUTPUT_DEB], check=True)
    print(f'Package created: {OUTPUT_DEB}')

except Exception as e:
    print(f'Failed to build deb package: {str(e)}')
    exit(1)
