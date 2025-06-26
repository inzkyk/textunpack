import os
import shutil
import glob
import sys
import re
import subprocess

root = os.path.dirname(os.path.normpath(__file__))
os.chdir(root)

sys.stdin.reconfigure(encoding="utf-8")  # type: ignore
sys.stdout.reconfigure(encoding="utf-8")  # type: ignore
sys.stderr.reconfigure(encoding="utf-8")  # type: ignore


def contains_path(path_list: list[str], path: str) -> bool:
  for element in path_list:
    if os.path.samefile(element, path):
      return True
  return False


def get_impl_path(header_path: str) -> str:
  base, ext = os.path.splitext(header_path)
  impl_ext = ""
  if ext == ".hpp":
    impl_ext = ".cpp"
  elif ext == ".h":
    impl_ext = ".c"
  else:
    assert False
  return base + impl_ext


class Amalgamator:
  include_regex = re.compile(r"""\#include\s(?:<|")(.*)(?:>|")""")

  def __init__(self, main_filename: str, include_dirs: list[str], files_to_ignore: list[str], additional_header_lines: list[str], show_log: bool):
    self.files_to_ignore: list[str] = []
    self.include_dirs = include_dirs
    self.addtional_header_lines = additional_header_lines
    self.show_log = show_log
    self.impl_files: list[str] = []
    self.outputs = []
    self.referenced_files = []

    self.skip_test = False
    for line in additional_header_lines:
      if line == "#define ix_DO_TEST 0\n":
        self.skip_test = True

    for f in files_to_ignore:
      f_fullpath = self.find_header_fullpath(f)
      assert f_fullpath is not None
      self.files_to_ignore.append(f_fullpath)

    self.process_file(main_filename)

    while len(self.impl_files) != 0:
      impl_file = self.impl_files.pop(0)
      self.process_file(impl_file)

    self.outputs = additional_header_lines + self.outputs
    self.referenced_files.sort()

  def find_header_fullpath(self, header_name: str) -> str | None:
    for include_dir in self.include_dirs:
      path = include_dir + header_name
      if os.path.exists(path):
        return path
    return None

  def process_file(self, filename: str):
    if self.show_log:
      print(f"[amalgamator.py] Processing {filename}")
    with open(filename, "r", encoding="utf8") as f:
      lines = f.readlines()

    self.outputs.append(f"// FILE BEGIN: {filename}\n")
    num_nested_pp_block = 0
    skipping_blocks = []

    for line in lines:
      if "#pragma once" in line:
        continue

      if line.startswith("extern template"):
        continue

      if line.startswith("template") and line[-2] == ";":
        continue

      if ("ix_Windows.hpp" not in filename) and line.startswith("#define ix_WIN_INCLUDE_"):
        self.addtional_header_lines.append(line)
        continue

      if line.startswith("#if"):
        num_nested_pp_block += 1
        # print(f"skipping block begin ({num_nested_pp_block}): {line}", end="")
        if (self.skip_test and line == "#if ix_DO_TEST\n") or line == "#if 0\n":
          skipping_blocks.append(num_nested_pp_block)
          continue
      elif line.startswith("#endif"):
        skipping_block_end = num_nested_pp_block in skipping_blocks
        # print(f"skipping block end   ({num_nested_pp_block}): {line}", end="")
        num_nested_pp_block -= 1
        if skipping_block_end:
          skipping_blocks.remove(num_nested_pp_block + 1)
          continue

      if len(skipping_blocks) != 0:
        continue

      m = Amalgamator.include_regex.match(line)
      if m is None:
        self.outputs.append(line)
        continue

      header_name = m[1]  # "foo.hpp"
      header_fullpath = self.find_header_fullpath(header_name)
      if self.show_log:
        print(f"{filename} includes {header_name}")
      if header_fullpath is None:
        if self.show_log:
          print(f"[amalgamator.py] Header file not found (make sure this is a system header): {header_name}")
        self.outputs.append(line)
        continue

      skip = contains_path(self.files_to_ignore, header_fullpath)
      if skip:
        if self.show_log:
          print(f"[amalgamator.py] Skipping {header_fullpath}")
        continue

      self.files_to_ignore.append(header_fullpath)
      self.process_file(header_fullpath)

      impl_fullpath = get_impl_path(header_fullpath)
      if not os.path.isfile(impl_fullpath):
        if self.show_log:
          print(f"[amalgamator.py] Implementation file not found: {header_fullpath}")
        continue

      skip = contains_path(self.files_to_ignore, impl_fullpath)
      if skip:
        if self.show_log:
          print(f"[amalgamator.py] Skipping {impl_fullpath}")
        continue

      self.impl_files.append(impl_fullpath)
      self.files_to_ignore.append(impl_fullpath)

    assert filename not in self.referenced_files
    self.referenced_files.append(filename)
    self.outputs.append(f"// FILE END: {filename}\n")
    assert num_nested_pp_block == 0


def amalgamate(main_filename: str, include_dirs: list[str], files_to_ignore: list[str], additional_header_lines: list[str], show_log: bool):
  return Amalgamator(main_filename, include_dirs, files_to_ignore, additional_header_lines, show_log)


def copy_dir_from_private_repository(src: str):
  if not os.path.isdir(src):
    print(f'Private "{src}" does not exist. Who are you?')
    return
  assert os.path.isdir(src)
  dst = src.replace("/code/cxx/", "./")
  shutil.rmtree(dst)
  shutil.copytree(src, dst, ignore=shutil.ignore_patterns("build", ".cache"))
  for x in glob.glob(dst + "*"):
    source_file = x.endswith((".hpp", ".cpp", ".h", ".c"))
    license_file = "LICENSE.txt" in x
    notice_file = "NOTICE.txt" in x
    if not (source_file or license_file or notice_file):
      if os.path.isfile(x):
        os.remove(x)
      elif os.path.isdir(x):
        shutil.rmtree(x)


def main():
  copy_dir_from_private_repository("/code/cxx/src/ix/")
  copy_dir_from_private_repository("/code/cxx/src/textunpack/")
  copy_dir_from_private_repository("/code/cxx/external/bx/")
  copy_dir_from_private_repository("/code/cxx/external/Microsoft_STL/")
  copy_dir_from_private_repository("/code/cxx/external/pdqsort/")
  copy_dir_from_private_repository("/code/cxx/external/sokol_time/")
  copy_dir_from_private_repository("/code/cxx/external/wyhash/")

  project_name = "textunpack"
  main_filename = "./src/textunpack/textunpack_cli.cpp"
  include_dirs = [
    "./src/ix/",
    "./src/textunpack/",
    "./external/sokol_time/",
  ]
  files_to_ignore = []
  additional_header_lines = [
    "#define UNICODE\n",
    "#define _UNICODE\n",
    "#define SOKOL_TIME_IMPL\n",
    "#define ix_SHIP 1\n",
    "#define ix_DO_TEST 0\n",
  ]

  amalgam = amalgamate(
    main_filename,
    include_dirs,
    files_to_ignore,
    additional_header_lines,
    "--log" in sys.argv,
  )

  amalgam_filename = "amalgam.cpp"
  amalgam_str = "".join(amalgam.outputs)
  empty_file_regex = re.compile("// FILE BEGIN: (.*?)\n+// FILE END: \\1\n")
  amalgam_str = empty_file_regex.sub("", amalgam_str)
  assert amalgam_str == empty_file_regex.sub("", amalgam_str)  # インクルードに無駄があると落ちる
  with open(amalgam_filename, "w+", encoding="utf-8", newline="\n") as f:
    f.write(amalgam_str)

  if "--print-referenced-files" in sys.argv:
    for referenced_file in amalgam.referenced_files:
      print(referenced_file)

  if "--compile" in sys.argv:
    ext = ".exe" if os.name == "nt" else ""
    subprocess.run(f"clang++ {amalgam_filename} -o textunpack{ext} -std=c++17 -O3")

  # CMakeLists.txt を作る
  additional_header_lines = [
    "#define UNICODE\n",
    "#define _UNICODE\n",
    "#define SOKOL_TIME_IMPL\n",
    "#define ix_SHIP 0\n",
    "#define ix_DO_TEST 1\n",  # テスト付きビルドで必要なファイルを収集する
  ]
  amalgam = amalgamate(
    main_filename,
    include_dirs,
    files_to_ignore,
    additional_header_lines,
    "--log" in sys.argv,
  )

  with open("CMakeLists.txt", "w+", encoding="utf-8", newline="\n") as f:
    f.write(f"cmake_minimum_required(VERSION 3.16)\n")
    f.write(f"project({project_name})\n\n")

    # 記録として残すために、使われていないファイル名もコメントとして残す
    source_files = glob.glob("./src/*/*") + glob.glob("./external/*/*")
    source_files = filter(lambda s: s.endswith((".hpp", ".cpp", ".h", ".c")), source_files)
    source_files = map(lambda s: s.replace("\\", "/"), source_files)
    source_files = list(source_files)
    source_files.sort()

    f.write(f"add_executable({project_name}\n")
    for source_file in source_files:
      if source_file in amalgam.referenced_files:
        f.write(f'  "{source_file}"\n')
      else:
        f.write(f'  # "{source_file}"\n')
    f.write(")\n\n")

    f.write(f"set_property(TARGET {project_name} PROPERTY CXX_STANDARD 17)\n")
    for definition in ["UNICODE", "_UNICODE", "ix_DO_TEST=1"]:
      f.write(f"target_compile_definitions({project_name} PRIVATE {definition})\n")
    for include_dir in include_dirs:
      f.write(f'target_include_directories({project_name} PRIVATE "{include_dir}")\n')

    f.write(f"""if (MSVC)
  target_compile_options(textunpack PRIVATE /utf-8)
  set_directory_properties(PROPERTY VS_STARTUP_PROJECT {project_name})
endif ()
""")

  # THIRD_PARTY_LICENSES.txt を作る
  separator = "===============================================================================\n"
  with open("THIRD_PARTY_LICENSES.txt", "w+", encoding="utf-8", newline="\n") as f:
    for external_dir in glob.glob("./external/*"):
      assert os.path.isdir(external_dir)

      license_file = external_dir + "/LICENSE.txt"
      assert os.path.isfile(license_file)
      f.write(separator)
      with open(license_file, "r", encoding="utf-8") as g:
        f.write(g.read())

      notice_file = external_dir + "/NOTICE.txt"
      if os.path.isfile(notice_file):
        f.write(separator)
        with open(notice_file, "r", encoding="utf-8") as g:
          f.write(g.read())

    f.write(separator)

  # 使われていないファイルを消す
  for source_file in source_files:
    if not contains_path(amalgam.referenced_files, source_file):
      os.remove(source_file)

  if "--cmake" in sys.argv:
    subprocess.run("cmake -B build -G Ninja")
    subprocess.run("cmake --build build")


if __name__ == "__main__":
  main()
