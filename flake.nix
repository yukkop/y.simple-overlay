{
  description = "Development environment with GCC and libX11";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }:
  {
    defaultPackage.x86_64-linux = with nixpkgs.legacyPackages.x86_64-linux;
    mkShell {
      buildInputs = [
        gcc
        xorg.libX11.dev
        xorg.libXft.dev
      ];

      shellHook = ''
      '';
    };
  };
}

