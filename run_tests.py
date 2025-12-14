import os
import subprocess
import glob

hw_folders = [f"HW{i:02d}" for i in range(2, 11)]

test_cases = {
    "HW02": [
        ("()", "Success\n"),
        ("{{", "1\n"),
        ("test({)}", "7\n"),
        ("test{()}", "Success\n")
    ],
    #- А остальные тесты??, - кто-то.
    #- И..., - голосом персонажа Pudge из компьтерной игры Dota 2.
}

for hw in hw_folders:
    if not os.path.exists(hw):
        print(f"Папка {hw} не найдена")
        continue
    
    # Ищем файлы .cpp в папке
    cpp_files = glob.glob(os.path.join(hw, "*.cpp"))
    if not cpp_files:
        print(f"В {hw} не найден .cpp файл")
        continue
    
    cpp_file = cpp_files[0]
    name = os.path.join(hw, "program")
    
    # Компилируем
    compile_cmd = f"g++ -std=c++17 {cpp_file} -o {name}"
    compile_result = subprocess.run(compile_cmd, shell=True, capture_output=True)
    
    if compile_result.returncode != 0:
        print(f"Ошибка компиляции {hw}: {compile_result.stderr.decode()}")
        continue
    
    # Запускаем тесты
    print(f"\n=== Тестирование {hw} ===")
    
    if hw in test_cases:
        for i, (input_data, expected) in enumerate(test_cases[hw], 1):
            result = subprocess.run(
                name,
                input=input_data.encode(),
                capture_output=True,
                shell=True
            )
            output = result.stdout.decode().strip()
            expected = expected.strip()
            
            if output == expected:
                print(f"  Тест {i}: ✓ PASSED")
            else:
                print(f"  Тест {i}: ✗ FAILED")
                print(f"    Ожидалось: {expected}")
                print(f"    Получено:  {output}")
    else:
        print(f"  Нет тестов для {hw}")
