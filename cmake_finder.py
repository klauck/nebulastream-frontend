import os

def find_cmakelists_files(directory):
    cmakelists_files = []
    # Walk through the directory and its subdirectories
    for root, dirs, files in os.walk(directory):
        # Check if 'CMakeLists.txt' exists in the files list
        if 'CMakeLists.txt' in files:
            # Construct the full path to the file and add it to the list
            cmakelists_files.append(os.path.join(root, 'CMakeLists.txt'))
    
    return cmakelists_files

import re

def search_cmake_files(cmake_files):
    # Regular expressions to match the patterns
    unity_library_pattern = re.compile(r'add_library_unity\(\s*([^\s)]+)')
    add_library_pattern = re.compile(r'add_library\(\s*([^\s)]+)')
    
    # List to store the results
    results = []

    # Loop through the provided list of CMakeLists.txt files
    for file_path in cmake_files:
        try:
            with open(file_path, 'r') as f:
                for line in f:
                    # Search for the patterns
                    unity_match = unity_library_pattern.search(line)
                    add_library_match = add_library_pattern.search(line)
                    
                    if unity_match:
                        # Extract the first parameter and append the result
                        results.append(f"target_link_libraries(playground {unity_match.group(1)})")
                    elif add_library_match:
                        # Extract the first parameter and append the result
                        results.append(f"target_link_libraries(playground {add_library_match.group(1)})")
        except Exception as e:
            print(f"Could not read file {file_path}: {e}")

    return results



# Usage example
if __name__ == "__main__":
    # Ask the user for the directory path to search
    directory_path = "./"
    
    # Search for CMakeLists.txt files
    found_files = find_cmakelists_files(directory_path)

    print(len(found_files))

    cmakes = search_cmake_files(found_files)

    print(len(cmakes))

    for cmake in cmakes:
        print(cmake)
