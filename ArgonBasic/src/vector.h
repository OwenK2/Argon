class Vec2 {
private:
public:
  int x;
  int y;
  Vec2();
  Vec2(double x, double y);
  ~Vec2();
  static std::vector<Vec2> sort(std::vector<Vec2> points);
};
