import sys


def parse_file(file):
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
    return clusters


def get_match_count(nclusters, oclusters):
    count = []
    for nc in nclusters:
        cnt = [0]*len(oclusters)
        i = 0
        while i < len(oclusters):
            for np in nc:
                if np in oclusters[i]:
                    cnt[i] = cnt[i] + 1
            i = i + 1
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
    print(count_matrix)
    priority_matrix = get_priorities(count_matrix=count_matrix)
    print(priority_matrix)
    assigned_cluster = [-1] * len(nclusters)
    for maxp in range(len(oclusters), 0, -1):
        for i in range(len(priority_matrix)):
            if assigned_cluster[i] == -1:
                assigned_cluster[i] = priority_matrix[i].index(maxp)
        # print(assigned_cluster)

        for i in range(len(assigned_cluster)):
            if assigned_cluster[i] >= 0:
                indices = [x for x in assigned_cluster[i:] if x == assigned_cluster[i]]
                # print(f"indices={indices}")
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
    print(assigned_cluster)
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
    if len(sys.argv) < 3:
        print(
            f"Usage: python3 {sys.argv[0]} {{new_cluster_file}} {{original_cluster_file}}")
        exit()

    with open(sys.argv[1], 'r') as file:
        nclusters = parse_file(file=file.readlines())
        print(sorted([len(x) for x in nclusters]))

    with open(sys.argv[2]) as file:
        oclusters = parse_file(file=file.readlines())
        print(sorted([len(x) for x in oclusters]))

    correct, incorrect = confusion_matrix(nclusters=nclusters, oclusters=oclusters)

    print(f"Correct Predictions = {correct}")
    print(f"Incorrect Predictions = {incorrect}")
