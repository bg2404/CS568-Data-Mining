import sys


def parse_file(file, data=None):

    clusters = []
    i = 0
    while i < len(file):
        cluster = []
        attrs = list(map(int, file[i].split()))
        for j in range(attrs[3]):
            i = i + 1
            cluster.append(int(file[i].split()[0]))
        i = i + 1
        if attrs[1] == 0:  # noise cluster
            clusters.append(cluster)

    mapping = [0]
    if data != None:
        cnt = 1
        for line in data[1:]:
            point = line.strip().split()
            skip = True
            for dim in point:
                if dim != "0":
                    skip = False
                    break
            if not skip:
                mapping.append(cnt)
                cnt = cnt + 1
            else:
                mapping.append(-1)
        for i in range(len(clusters)):
            for j in range(len(clusters[i])):
                clusters[i][j] = mapping[clusters[i][j]]

    return clusters


def get_match_count(nclusters, oclusters):
    count = []
    for nc in nclusters:
        cnt = [0]*len(oclusters)
        for i, oc in enumerate(oclusters):
            for np in nc:
                if np in oc:
                    cnt[i] = cnt[i] + 1
        count.append(cnt)
    return count


def get_priorities(count_matrix):
    priority_matrix = []
    for count_list in count_matrix:
        temp = sorted(count_list)
        temp = [temp.index(count)+1 for count in count_list]
        for i in range(len(temp)):
            for j in range(i+1, len(temp)):
                if temp[j] == temp[i]:
                    temp[j] = temp[j-1] + 1
        priority_matrix.append(temp)
    return priority_matrix


def confusion_matrix(nclusters, oclusters):
    count_matrix = get_match_count(nclusters=nclusters, oclusters=oclusters)
    priority_matrix = get_priorities(count_matrix=count_matrix)
    assigned_cluster = [-1] * len(nclusters)
    for maxp in range(len(oclusters), 0, -1):
        for i in range(len(priority_matrix)):
            if assigned_cluster[i] == -1:
                assigned_cluster[i] = priority_matrix[i].index(maxp)

        for i in range(len(assigned_cluster)):
            if assigned_cluster[i] >= 0:
                indices = [x for x in assigned_cluster[i:]
                           if x == assigned_cluster[i]]
                if len(indices) > 0:
                    maxidx = indices[0]
                    for idx in indices[1:]:
                        if len(nclusters[idx]) > len(nclusters[maxidx]):
                            maxidx = idx
                    for idx in indices:
                        if idx != maxidx:
                            assigned_cluster[idx] = -1
    correct_classifications = 0
    incorrect_classifications = 0
    # print("Cluster Assignment")
    # for i, nc in enumerate(nclusters):
    #     print(len(nc), len(oclusters[assigned_cluster[i]]),
    #           count_matrix[i][assigned_cluster[i]])
    for i in range(len(assigned_cluster)):
        j = assigned_cluster[i]
        if j == -1:
            incorrect_classifications = incorrect_classifications + \
                len(nclusters[i])
        else:
            correct_classifications = correct_classifications + \
                count_matrix[i][j]
            incorrect_classifications = incorrect_classifications + \
                len(nclusters[i]) - count_matrix[i][j]
    return [correct_classifications, incorrect_classifications]


if __name__ == '__main__':
    if len(sys.argv) < 2 or len(sys.argv) == 3:
        print(f"Usage1: python3 {sys.argv[0]} {{dataset}}")
        print(f"Note that in this case files should be present in the same directory and their names should be {{database_static.csv}} {{database_incr.csv}} {{database_updated.csv}}")
        print(f"\nUsage2: python3 {sys.argv[0]} {{static_file}} {{incr_file}} {{update_file}}")
        exit()
    elif len(sys.argv) == 2:
        static = sys.argv[1] + "_static.csv"
        incr = sys.argv[1] + "_incr.csv"
        updated = sys.argv[1] + "_updated.csv"
    elif len(sys.argv) == 4:
        static = sys.argv[1]
        incr = sys.argv[2]
        updated = sys.argv[3]
    with open(static, 'r') as file:
        nclusters = parse_file(file=file.readlines(), data=None)
        # print(static, [len(x) for x in nclusters])

    with open(incr) as file:
        with open(updated) as file2:
            oclusters = parse_file(file=file.readlines(), data=file2.readlines())
            # print(incr, [len(x) for x in oclusters])

    correct, incorrect = confusion_matrix(
        nclusters=nclusters, oclusters=oclusters)

    print(f"Dataset: {static[:-11]}")
    print(f"Correct Predictions = {correct}")
    print(f"Incorrect Predictions = {incorrect}")
    print()
