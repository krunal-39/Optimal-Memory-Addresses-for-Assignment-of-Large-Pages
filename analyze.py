import sys
import re
import subprocess

def run_command(command):
    try:
        subprocess.run(command, check=True, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"An error occurred while executing: {command}\nError: {e}")

def parse_perf_output(perf_file):
    base_addresses_dict = {}
    page_size = 2 * 1024 * 1024
    pattern = re.compile(r"(0x[0-9a-fx]+)\s+.*L2 miss")
    with open(perf_file, 'r') as file:
        for line in file:
            if line.startswith("#") or not line.strip():
                continue
            try:
                match = pattern.search(line)
                if match:
                    address = match.group(1) 
                    address_int = int(address, 16)  
                    base_address = (address_int // page_size) * page_size  
                    if base_address in base_addresses_dict:
                        base_addresses_dict[base_address] += 1  
                    else:
                        base_addresses_dict[base_address] = 1  
            except: 
                continue   
    return base_addresses_dict

def find_optimal_pages(page_accesses, n):
    sorted_pages = sorted(page_accesses.items(), key=lambda item: item[1], reverse=True)
    return [page[0] for page in sorted_pages[:n]]


def write_largepages_file(optimal_pages, output_file='largepages.txt'):
    with open(output_file, 'w') as f:
        for page in optimal_pages:
            f.write(f'{page}\n')  

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: main <give one argument as max no of deployable large pages>\n")
        sys.exit(1)

    n = int(sys.argv[1])

    commands_li = ['perf mem record ./main 24360', 'perf mem report > memory_accesses.txt']
    for cmd in commands_li:
        run_command(cmd)

    perf_file = 'memory_accesses.txt' 
    
    base_addresses_dict = parse_perf_output(perf_file) 
    optimal_pages_df = find_optimal_pages(base_addresses_dict, n)  
    write_largepages_file(optimal_pages_df)  