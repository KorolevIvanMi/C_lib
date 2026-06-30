from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import shutil
import os
import sys

class MakeBuild(build_ext):
    def run(self):
        # Проверяем флаг USE_FALLBACK
        use_fallback = os.environ.get('USE_FALLBACK', '').lower() in ('true', '1', 'yes')
        
        if use_fallback:
            print("Использую готовый .so файл")
            # Ищем .so в разных местах
            so_paths = ["lib/mylist.so", "build/mylist.so", "mylist.so"]
            for path in so_paths:
                if os.path.exists(path):
                    os.makedirs(self.build_lib, exist_ok=True)
                    shutil.copy2(path, self.build_lib)
                    print(f"Скопирован: {path} -> {self.build_lib}")
                    return
            print("Готовый .so не найден, пробую компилировать через make...")
        
        # Запускаем make
        print("Компиляция через make...")
        result = subprocess.run(['make', 'clean', 'all'], capture_output=True, text=True)
        print(result.stdout)
        if result.returncode != 0:
            print(result.stderr)
            raise RuntimeError("Make failed")
        
        # Проверяем, что .so создался
        so_file = "build/mylist.so"
        if not os.path.exists(so_file):
            raise RuntimeError(f"Не найден {so_file} после сборки")
        
        # Копируем в build/lib/ (для установки)
        os.makedirs(self.build_lib, exist_ok=True)
        shutil.copy2(so_file, self.build_lib)
        print(f"Собрано: {so_file} -> {self.build_lib}")

setup(
    name="mylist",
    version="1.0.0",
    ext_modules=[Extension('mylist', sources=[])],
    packages=[],
    cmdclass={"build_ext": MakeBuild},
)