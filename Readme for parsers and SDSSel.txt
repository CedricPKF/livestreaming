========== yelp3.cpp ==========

COMPILE: g++ yelp3.cpp -std=gnu++0x -O2 -o yelp3
COMMAND: ./yelp3 -business business.json -review review.json -tip tip.json -user yuser.json -v 100 -c 10000 [-euclid]

-business, -review, -tip, -user arguments are for the original input files

-v and -c are the maximum viewer and channels you want to sample

-euclid is a switch to use euclidean distance to measure the diversity, 


OUTPUT:
standard output: an input file for SDSSelrithm program

stderr: a reference file with original user/channel name


EXAMPLE:

./yelp3 -business yelp_academic_dataset_business.json -review yelp_academic_dataset_review.json -tip yelp_academic_dataset_tip.json -user yelp_academic_dataset_user.json -v 100 -c 100 > input.txt

./yelp3 -business yelp_academic_dataset_business.json -review yelp_academic_dataset_review.json -tip yelp_academic_dataset_tip.json -user yelp_academic_dataset_user.json -v 100 -c 10000 -euclid > input.txt

========== douban.cpp ==========

COMPILE: g++ douban3.cpp -std=gnu++0x -O2 -o douban3

COMMAND: ./douban3 -friend FRIEND_FILE -item ITEM_FILE -v V -c C

-friend and -item are the friendship input and item input

-v and -c are the maximum viewer and channels you want to sample


EXAMPLE:
./douban3 -friend douban_friendship.txt -item music_collection.csv -v 10 -c 10 > graph.txt 2> label.txt
========== SDSSel.cpp ==========

COMPILE: g++ SDSSel.cpp -std=gnu++0x -O2 -o SDSSel
COMMAND: ./SDSSel -h H_VALUE -p P_VALUE [-prune1] [-prune2] < INPUT

-h, -p are the parameters of the SDSSelrithm
they should be positive integers

-prune1, -prune2 are switches of the pruning strategies (PCP and CDP)

EXAMPLE:

./SDSSel -h 10 -p 1 -prune2 < INPUT (turn on prune2, h=10, p=1)
./SDSSel -h 1 -p 1 -prune1 -prune2 < INPUT (turn on both prunes, h = p = 1)


SAMPLE OUTPUT:
0.024000 0 1 74.947144 2 1.010000
16 100
1 3 4 5 6 9 13 23 25 29 30 35 36 37 52 67
101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200

EXPALIN Of SAMPLE OUTPUT

0.024000 0 1 74.947144 2 1.010000
# the first line are formed by 6 numbers:
# 1.execution time(second, float)
# 2.does the solution meet all the constraints (0 or 1)
# 3.does the solution meet all the constraints, without h-dense constraint (0 or 1)
# 4.the objective value
# 5.the farest distance of viewers that we chose (integer)
# 6.the average number of each channel's preference edges 


16 100
#in second line there are two numbers, the set size of viewers and size of channels

#the third and forth lines, are the viewers and channels in the solution
