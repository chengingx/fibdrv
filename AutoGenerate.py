import os 
import pandas as pd
import math
from tqdm import tqdm

def process_data(l, pd, name):

    print(f"Processing {name} data...")
    mean, std = pd.mean(axis = 1), pd.std(axis = 1)
    
    for i in tqdm(range(len(pd.index))):
        sum = 0
        cnt = 0
        for j in pd.iloc[i]:
            if abs(j - mean[i]) < 2 * std[i]:
                sum = sum + j;
                cnt = cnt + 1;
        sum = sum / cnt;
        l.append(sum)

if __name__ == '__main__':

    times = 10000
    
    kernel = pd.DataFrame(columns = range(times))
    user = pd.DataFrame(columns = range(times))
    kernel_to_user = pd.DataFrame(columns = range(times))

    kernel_cost = []
    user_cost = []
    kernel_to_user_cost = []

    print("Generating data...")
    for i in tqdm(range(times)):
        os.system('sudo taskset -c 0 ./client > ./data/' + str(i))

    print("Reading data...")
    for i in tqdm(range(times)):
        f = open('./data/' + str(i), 'r')
        content = f.read()
        raw = content.split('\n')
        raw.remove('')
    
        kernel_tmp = []
        user_tmp = []
        kernel_to_user_tmp = []
    
        for j in range(len(raw)):
            numbers = raw[j].split('    ')
            kernel_tmp.append(int(numbers[1]))
            user_tmp.append(int(numbers[2]))
            kernel_to_user_tmp.append(int(numbers[3]))
        kernel[i] = kernel_tmp
        user[i] = user_tmp
        kernel_to_user[i] = kernel_to_user_tmp

    process_data(kernel_cost, kernel, "kernel")
    process_data(user_cost, user, "user")
    process_data(kernel_to_user_cost, kernel_to_user, "kernel to user")

    out = []
    for i, j, k, l in zip(range(len(kernel_cost)), kernel_cost, user_cost, kernel_to_user_cost):
	    out.append('{} {} {} {}'.format(i, j, k, l))

    # output the result
    f = open('output_fast.txt', 'w')
    print('file write: {}'.format(f.write('\n'.join(out))))
    f.close()
    print("Finish !")
