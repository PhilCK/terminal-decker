#ifndef NETWORK_INCLUDED_748A28B6_BD98_4080_BB4B_79FFC95636ED
#define NETWORK_INCLUDED_748A28B6_BD98_4080_BB4B_79FFC95636ED


class Network
{
public:

  explicit      Network();
  void          think(const float dt);
  void          add_system(int system);

}; // class


#endif // include guard