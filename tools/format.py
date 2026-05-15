import os
import subprocess
import sys

def format_files():
    extensions = ('.cpp', '.cc', '.h', '.hpp')
    directories = ('src', 'std', 'include')
    
    files_to_format = []
    for directory in directories:
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith(extensions):
                    files_to_format.append(os.path.join(root, file))
    
    if not files_to_format:
        print("No files found to format.")
        return

    print(f"Formatting {len(files_to_format)} files...")
    try:
        subprocess.run(['clang-format', '-i'] + files_to_format, check=True)
        print("Formatting complete!")
    except FileNotFoundError:
        print("Error: clang-format not found. Please install it (e.g., sudo apt install clang-format).")
    except subprocess.CalledProcessError as e:
        print(f"Error during formatting: {e}")

if __name__ == "__main__":
    format_files()
