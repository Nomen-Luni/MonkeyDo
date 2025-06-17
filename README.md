# *I AM ACTIVLY DEVELOPING ON THE MAIN BRANCH (naughty!) - use the last tagged version if you want functional code (i.e. current built release)*

# MonkeyDo

A powerful batch file renaming utility built with the Qt technology stack.

![MonkeyDo.png](./Images/MonkeyDo.png)





![Batch_renamer.png](./Images/screenshot.png)

#### Creation of installer:

- Build the release executable in QtCreator.

- For Ubuntu and other Debian based Linux distributions:
  
  - Navigate to the ''Deb Packaging' directory and run ''MakeDeb' to create the .deb distribution package.

#### Installation:

- From Dolphin File Manager in KDE Plasma:
  
  - From the Dolphin menu select 'Settings->Configure Dolphin->Context Menu->Download New Services' and search for 'Bionic Batch Renamer.'
    - Select the .deb file to install on Ubuntu and Debian based distros.
    - Select the .tar file to install on Arch based distros.
  - This will add the option 'Batch rename' to the right-click menu in the file manager when multiple files are selected.

- On Ubuntu and other Debian based Linux distributions:
  
  - Double-click the .deb installation file and install via your package manager. The executable will be located at /usr/local/bin/Batch_Renamer.

- On Arch based Linux distributions:
  
  - Via [AUR package (Stable Version)](https://aur.archlinux.org/packages/bionic-batch-renamer) or [AUR package (Development Version)](https://aur.archlinux.org/packages/bionic-batch-renamer-git)
  - **_or_** Double-click the .tar installation file and install via your package manager

#### Authors:

Thanks to everyone who has helped out:

- Lee Cooper (chief honcho)
- Translators:
  - Heimen Stoffels (Dutch)
  - Jan Rathmann (German)
- Installers:
  - Carl F (Arch installer)
  - Oliver Kahrmann (AUR build script maintainer)

#### Props:

- The authors of [Thunar's 'bulk renamer'](https://docs.xfce.org/xfce/thunar/bulk-renamer/start) plugin, which inspired MonkeyDo's interface.
