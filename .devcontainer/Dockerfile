FROM debian:bookworm

RUN apt-get update \
 && apt-get install -y \
        build-essential git checkinstall libssl-dev openssl software-properties-common \
        sudo curl procps gdb clang \
        crossbuild-essential-arm64

## vscode user
RUN useradd -ms /bin/bash vscode \
 && apt-get update \
 && apt-get install -y sudo \
 && echo vscode ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/vscode \
 && chmod 0440 /etc/sudoers.d/vscode

USER vscode
WORKDIR /home/vscode

# Homebrew
RUN curl -fsSL "https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh" | NONINTERACTIVE=1 /bin/bash \
 && eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)" \
 && brew update \
 && brew install cmake ninja python@3.12 git rustup
ENV PATH="/home/linuxbrew/.linuxbrew/bin:/home/linuxbrew/.linuxbrew/sbin:${PATH}"

# Rust
RUN rustup-init -y \
 && rustup target add aarch64-unknown-linux-gnu

# Python and Conan
RUN python3.12 -m venv /home/vscode/venv \
 && /home/vscode/venv/bin/python -m pip install -U pip setuptools wheel conan
ENV VIRTUAL_ENV="/home/vscode/venv"
ENV PATH="/home/vscode/venv/bin:${PATH}"
