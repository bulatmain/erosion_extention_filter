import os

parent_dir = "/home/bulat/mai/studying/sem3/OSLabs/Lab2/"

os.chdir(parent_dir)
print(os.getcwd())

build_dir = parent_dir + "build/"

research_dir = parent_dir + "research/"

if not os.path.isdir(build_dir):
    os.mkdir(build_dir)

os.chdir(build_dir)
os.system("cmake ../")
os.system("make")


image_file = research_dir + "image_30x30"
result_file = research_dir + "results"

K = 1
threadsCount = 8

for i in range(1, threadsCount + 1):
    call = "./exec " + image_file + " 30 30 " + str(K) + " " + str(i) + " " + result_file
    print(call)
    os.system(call)