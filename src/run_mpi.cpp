#include <../include/dataloader.h>
#include <../include/graph.h>
#include "../include/common.h"
#include "../include/pattern.h"
#include "../include/schedule.h"

#include <mpi.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 5) {
    printf("Usage: %s dataset_type graph_file pattern_size pattern_adjacency_matrix number_of_threads_per_process\n",
           argv[0]);
    printf("Example(Triangle counting on dataset WikiVote) : \n");
    printf("%s Wiki-Vote ../../dataset/wiki-vote_input 3 011101110\n", argv[0]);
    return 0;
  }

  const std::string type = argv[1];
  const std::string path = argv[2];
  int pattern_size = atoi(argv[3]);
  char *adj_mat = argv[4];
  int thread_count = atoi(argv[5]);

  // comments in include/schedule.h explain the meaning of these parameters.
  int use_in_exclusion_optimize = 1;
  int performance_type = 1;
  int restricts_type = 1;

  Graph *g;

  // load data
  DataLoader D;
  DataType my_type;
  GetDataType(my_type, type);
  if (my_type == DataType::Invalid) {
    printf("Dataset not found!\n");
    return 0;
  }
  if (D.load_data(g, my_type, path.c_str()) != true) {
    printf("Load error.");
    return -1;
  }
  printf("load data successfully!\n");
  fflush(stdout);

  // build schedule
  double start_time = get_wall_time();
  Pattern p(pattern_size, adj_mat);
  bool is_pattern_valid;
  Schedule schedule(p, is_pattern_valid, performance_type, restricts_type, use_in_exclusion_optimize, g->v_cnt,
                    g->e_cnt);
  double end_time = get_wall_time();
  double schedule_time = end_time - start_time;
  printf("build schedule successfully!\n");
  fflush(stdout);

  // distributed subgraph matching
  start_time = get_wall_time();
  long long ans_aggressive = g->pattern_matching_mpi(schedule, thread_count);
  end_time = get_wall_time();
  printf("schedule time, counting time, answer\n");
  printf("LOG,%.6lf,%.6lf,%lld\n", schedule_time, end_time - start_time, ans_aggressive);
  fflush(stdout);

  delete g;
  return 0;
}
