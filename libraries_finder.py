import os
import re

def search_cmake_files(directory):
    # Regular expressions to match the patterns
    unity_library_pattern = re.compile(r'add_library_unity\(\s*([^\s)]+)')
    add_library_pattern = re.compile(r'add_library\(\s*([^\s)]+)')
    
    # List to store the results
    results = []

    # Walk through the directory and subdirectories
    for root, _, files in os.walk(directory):
        for file in files:
            if file == 'CMakeLists.txt':
                file_path = os.path.join(root, file)
                
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

    # Output the results to console
    for result in results:
        print(result)

# Example usage
directory_to_search = './'  # Change this to your directory path
search_cmake_files(directory_to_search)
