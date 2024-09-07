import os

def search_cmake_files(directory):
    # List to store the results
    results = []

    # Walk through the directory and subdirectories
    for root, _, files in os.walk(directory):
        for file in files:
            if file == 'CMakeLists.txt':
                file_path = os.path.join(root, file)

                # # Skip if it's not the specific file you're interested in
                # if file_path != "./src/core_functions/scalar/date/CMakeLists.txt":
                #     continue

                # print(file_path)
                
                # Read the whole file content at once
                with open(file_path, 'r') as f:
                    file_content = f.read()
                    
                    # Search for `add_library_unity` and `add_library` in the whole text
                    while 'add_library_unity(' in file_content:
                        # Find the first occurrence and extract the argument
                        start_index = file_content.find('add_library_unity(') + len('add_library_unity(')
                        end_index = file_content.find(')', start_index)
                        lib_name = file_content[start_index:end_index].strip().split()[0]
                        results.append(f"target_link_libraries(playground {lib_name})")
                        
                        # Move the search index forward to find the next occurrence
                        file_content = file_content[end_index:]

                    # Similar process for `add_library`
                    while 'add_library(' in file_content:
                        start_index = file_content.find('add_library(') + len('add_library(')
                        end_index = file_content.find(')', start_index)
                        lib_name = file_content[start_index:end_index].strip().split()[0]
                        results.append(f"target_link_libraries(playground {lib_name})")
                        
                        # Move the search index forward to find the next occurrence
                        file_content = file_content[end_index:]

    # Output the results to console
    for result in results:
        print(result)

# Example usage
directory_to_search = './'  # Change this to your directory path
search_cmake_files(directory_to_search)
