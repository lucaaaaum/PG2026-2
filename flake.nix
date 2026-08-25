{
  description = "Projetos relacionados à cadeira de Processamento Gráfico: Fundamentos";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

        stb = pkgs.fetchFromGitHub {
          owner = "nothings";
          repo = "stb";
          rev = "2c980bb59875b0d32144a71867fbdebb2f77cd20";
          hash = "sha256-vA5RZLte4gf5/NkbWT3VNzGVD04kyVTHPeEZwxNnxi0="; # ver nota abaixo
        };

        shellPackages = with pkgs; [
          cmake
          glfw
          glm
          libGL
          gcc
        ];
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = shellPackages;

          shellHook = ''
            export CPATH="${stb}:$CPATH"
          '';
        };
      }
    );
}
