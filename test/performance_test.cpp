#include <gtest/gtest.h>
#include <../include/graph.h>
#include <../include/dataloader.h>
#include "../include/pattern.h"
#include "../include/schedule.h"
#include "../include/common.h"

#include <iostream>
#include <string>
#include <algorithm>

TEST(performance_test, performance_patents_qg2) {
    Graph *g;
    DataLoader D;
    
    std::string type = "Patents";
    std::string path = "/home/zms/patents_input";
    DataType my_type;
    if(type == "Patents") my_type = DataType::Patents;
    else {
        printf("invalid DataType!\n");
    }

    std::vector<long long> graph_degree_info;
    std::vector<long long> graph_size_info;
    
    int pattern_size = 4;
    int pattern_diameter = 2;
    int max_pattern_degree = 2;
    
    Pattern pattern(pattern_size);
    pattern.add_edge(0, 1);
    pattern.add_edge(0, 3);
    pattern.add_edge(1, 2);
    pattern.add_edge(2, 3);

    ASSERT_EQ(D.load_data(g,my_type,path.c_str(), pattern_size, max_pattern_degree, pattern_diameter, graph_degree_info, graph_size_info),true); 
    
    printf("Load data success!\n");
    fflush(stdout);

    bool is_valid;
    Schedule schedule(pattern, is_valid, true, graph_degree_info, graph_size_info);

    ASSERT_EQ(is_valid, true);

    std::vector< std::pair<int,int> > pairs;
    schedule.aggresive_optimize( schedule.get_adj_mat_ptr(), pairs);
    schedule.add_restrict(pairs);

    int thread_num = 24;
    double t1,t2;
    long long ans;
    
    t1 = get_wall_time();
    ASSERT_NE( ans = g->pattern_matching(schedule, thread_num), 0);
    t2 = get_wall_time();

    printf("%lld %.6lf\n", ans, t2 - t1);

    delete g;
}