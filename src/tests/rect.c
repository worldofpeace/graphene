#include <graphene.h>
#include <mutest.h>

static void
rect_init (mutest_spec_t *spec)
{
  graphene_rect_t *r;
  graphene_rect_t s;

  r = graphene_rect_init (graphene_rect_alloc (), 0.f, 0.f, 10.f, 10.f);
  mutest_expect ("init to initialize the origin",
                 mutest_bool_value (graphene_point_equal (&r->origin, &GRAPHENE_POINT_INIT (0.f, 0.f))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("init to initialize the size",
                 mutest_bool_value (graphene_size_equal (&r->size, &GRAPHENE_SIZE_INIT (10.f, 10.f))),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_init_from_rect (&s, r);
  mutest_expect ("initializing from a rectangle creates an identical rectangle",
                 mutest_bool_value (graphene_rect_equal (&s, r)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_free (r);
}

static void
rect_normalize (mutest_spec_t *spec)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (10.f, 10.f, -10.f, -10.f);
  graphene_rect_t s = GRAPHENE_RECT_INIT ( 0.f,  0.f,  10.f,  10.f);
  graphene_rect_t t;

  graphene_rect_normalize_r (&r, &t);

  mutest_expect ("normalizing(10, 10, -10, -10) will put the origin in (0, 0)",
                 mutest_bool_value (graphene_point_equal (&t.origin, &GRAPHENE_POINT_INIT (0, 0))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("normalizing(10, 10, -10, -10) will make the width positive",
                 mutest_float_value (t.size.width),
                 mutest_to_be_greater_than, 0.0,
                 mutest_to_be_close_to, 10.0, 0.0001,
                 NULL);
  mutest_expect ("normalizing(10, 10, -10, -10) will make the height positive",
                 mutest_float_value (t.size.height),
                 mutest_to_be_greater_than, 0.0,
                 mutest_to_be_close_to, 10.0, 0.0001,
                 NULL);
  mutest_expect ("equality to operate on normalized rectangles",
                 mutest_bool_value (graphene_rect_equal (&r, &s)),
                 mutest_to_be_true,
                 NULL);
}

static void
rect_equal (mutest_spec_t *spec)
{
  graphene_rect_t *r, *s;

  r = graphene_rect_init (graphene_rect_alloc (), 0.f, 0.f, 10.f, 10.f);
  s = graphene_rect_init (graphene_rect_alloc (), 1.f, 1.f,  9.f,  9.f);

  mutest_expect ("a rectangle to be equal it itself",
                 mutest_bool_value (graphene_rect_equal (r, r)),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("a rectangle to not be equal to null",
                 mutest_bool_value (graphene_rect_equal (r, NULL)),
                 mutest_to_be_false,
                 NULL);
  mutest_expect ("null to not be equal to a rectangle",
                 mutest_bool_value (graphene_rect_equal (NULL, r)),
                 mutest_to_be_false,
                 NULL);
  mutest_expect ("two rectangles with different values to not be equal",
                 mutest_bool_value (graphene_rect_equal (r, s)),
                 mutest_to_be_false,
                 NULL);

  graphene_rect_free (r);
  graphene_rect_free (s);
}

static void
rect_contains_point (mutest_spec_t *spec)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);
  graphene_point_t p;

  graphene_rect_get_top_left (&r, &p);
  mutest_expect ("a rectangle to contain its top left corner",
                 mutest_bool_value (graphene_rect_contains_point (&r, &p)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_get_top_right (&r, &p);
  mutest_expect ("a rectangle to contain its top right corner",
                 mutest_bool_value (graphene_rect_contains_point (&r, &p)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_get_bottom_right (&r, &p);
  mutest_expect ("a rectangle to contain its bottom right corner",
                 mutest_bool_value (graphene_rect_contains_point (&r, &p)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_get_bottom_left (&r, &p);
  mutest_expect ("a rectangle to contain its bottom left corner",
                 mutest_bool_value (graphene_rect_contains_point (&r, &p)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_get_center (&r, &p);
  mutest_expect ("a rectangle to contain its center",
                 mutest_bool_value (graphene_rect_contains_point (&r, &p)),
                 mutest_to_be_true,
                 NULL);

  graphene_point_init (&p, -1.f, 11.f);
  mutest_expect ("a rectangle to not contain a point outside its boundaries",
                 mutest_bool_value (graphene_rect_contains_point (&r, &p)),
                 mutest_to_be_false,
                 NULL);
}

static void
rect_contains_rect (mutest_spec_t *spec)
{
  graphene_rect_t *r, *s;

  r = graphene_rect_init (graphene_rect_alloc (), 0.f, 0.f, 10.f, 10.f);
  s = graphene_rect_init (graphene_rect_alloc (), 1.f, 1.f,  9.f,  9.f);

  mutest_expect ("a rectangle to contain itself",
                 mutest_bool_value (graphene_rect_contains_rect (r, r)),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("the smaller rectangle to not contain the larger one",
                 mutest_bool_value (graphene_rect_contains_rect (s, r)),
                 mutest_to_be_false,
                 NULL);
  mutest_expect ("the larger rectangle to contain the smaller one",
                 mutest_bool_value (graphene_rect_contains_rect (r, s)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_free (r);
  graphene_rect_free (s);
}

static void
rect_intersect (mutest_spec_t *spec)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);
  graphene_rect_t s = GRAPHENE_RECT_INIT (5.f, 5.f, 15.f, 15.f);
  graphene_rect_t q = GRAPHENE_RECT_INIT (11.f, 11.f, 2.f, 2.f);
  graphene_rect_t i, j;

  mutest_expect ("intersection between (0, 0, 10, 10) and (5, 5, 15, 15) not be empty",
                 mutest_bool_value (graphene_rect_intersection (&r, &s, &i)),
                 mutest_not, mutest_to_be_false,
                 NULL);

  mutest_expect ("origin of intersection to be at (5, 5)",
                 mutest_bool_value (graphene_point_equal (&i.origin, &GRAPHENE_POINT_INIT (5, 5))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("size of intersection to be (5, 5)",
                 mutest_bool_value (graphene_size_equal (&i.size, &GRAPHENE_SIZE_INIT (5, 5))),
                 mutest_to_be_true,
                 NULL);

  mutest_expect ("intersection between (5, 5, 5, 5) and (11, 11, 2, 2) to be empty",
                 mutest_bool_value (graphene_rect_intersection (&i, &q, &j)),
                 mutest_not, mutest_to_be_true,
                 NULL);
  mutest_expect ("empty intersection to be a degenerate rectangle",
                 mutest_bool_value (graphene_rect_equal (&j, graphene_rect_zero ())),
                 mutest_to_be_true,
                 NULL);
}

static void
rect_union (mutest_spec_t *spec)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);
  graphene_rect_t s = GRAPHENE_RECT_INIT (5.f, 5.f, 15.f, 15.f);
  graphene_rect_t u;

  graphene_rect_union (&r, &s, &u);
  mutest_expect ("union to take the minimum normalize origin",
                 mutest_bool_value (graphene_point_equal (&u.origin, &r.origin)),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("union to take the maximum normalized size",
                 mutest_bool_value (graphene_size_equal (&u.size, &GRAPHENE_SIZE_INIT (20.f, 20.f))),
                 mutest_to_be_true,
                 NULL);

  mutest_expect ("union rectangle to contain original rectangle A",
                 mutest_bool_value (graphene_rect_contains_rect (&u, &r)),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("union rectangle to contain original rectangle B",
                 mutest_bool_value (graphene_rect_contains_rect (&u, &s)),
                 mutest_to_be_true,
                 NULL);
}

static void
rect_offset (mutest_spec_t *spec)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);

  graphene_rect_offset (&r, 5.f, 5.f);
  mutest_expect ("offset to move the origin",
                 mutest_bool_value (graphene_point_equal (&r.origin, &GRAPHENE_POINT_INIT (5.f, 5.f))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("offset to keep the size",
                 mutest_bool_value (graphene_size_equal (&r.size, &GRAPHENE_SIZE_INIT (10.f, 10.f))),
                 mutest_to_be_true,
                 NULL);
}

static void
rect_inset (mutest_spec_t *spec)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);

  graphene_rect_inset (&r, 2.f, 2.f);
  mutest_expect ("inset(2, 2) to move the origin",
                 mutest_bool_value (graphene_point_equal (&r.origin, &GRAPHENE_POINT_INIT (2.f, 2.f))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("inset(2, 2) to shrink the size",
                 mutest_bool_value (graphene_size_equal (&r.size, &GRAPHENE_SIZE_INIT (6.f, 6.f))),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_inset (&r, 2.f, -2.f);
  mutest_expect ("inset(2, -2) to move back the origin",
                 mutest_float_value (r.origin.y),
                 mutest_to_be_close_to, 0.0, 0.0001,
                 NULL);
  mutest_expect ("inset(2, -2) to shrink the width",
                 mutest_float_value (r.size.width),
                 mutest_to_be_close_to, 2.0, 0.0001,
                 NULL);
  mutest_expect ("inset(2, -2) to expand the height",
                 mutest_float_value (r.size.height),
                 mutest_to_be_close_to, 10.0, 0.0001,
                 NULL);
}

static void
rect_round_to_pixel (mutest_spec_t *spec)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.5f, 1.9f,  9.3f, 8.7f);
  graphene_rect_t s = GRAPHENE_RECT_INIT (0.0f, 1.0f, 10.0f, 9.0f);

  graphene_rect_round_to_pixel (&r);
  mutest_expect ("rounded up rectangle to contain original rectangle",
                 mutest_bool_value (graphene_rect_contains_rect (&s, &r)),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("rounding is stable",
                 mutest_bool_value (graphene_rect_equal (&r, &s)),
                 mutest_to_be_true,
                 NULL);
}

static void
rect_expand (mutest_spec_t *spec)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 100.f, 100.f);
  graphene_point_t p;
  graphene_size_t s;
  graphene_rect_t check;

  graphene_rect_expand (&r, graphene_point_init (&p, -10.f, -10.f), &check);
  graphene_size_init (&s, 110.f, 110.f);
  mutest_expect ("expanding behind the origin changes the origin",
                 mutest_bool_value (graphene_point_equal (&p, &(check.origin))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("expanding behind the origin does not change the size",
                 mutest_bool_value (graphene_size_equal (&s, &(check.size))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("expanded rectangle contains original rectangle",
                 mutest_bool_value (graphene_rect_contains_rect (&check, &r)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_expand (&r, graphene_point_init (&p, 150.f, 150.f), &check);
  graphene_size_init (&s, 150.f, 150.f);
  mutest_expect ("expanding after the anti-origin does not change the origin",
                 mutest_bool_value (graphene_point_equal (&r.origin, &check.origin)),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("expanding after the anti-origin changes the size",
                 mutest_bool_value (graphene_size_equal (&s, &(check.size))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("expanded rectangle contains original rectangle",
                 mutest_bool_value (graphene_rect_contains_rect (&check, &r)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_expand (&r, graphene_point_init (&p, 50.f, 150.f), &check);
  mutest_expect ("expanding half in/half out does not change the origin",
                 mutest_bool_value (graphene_point_equal (&r.origin, &(check.origin))),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("expanding horizontally inside the rectangle to not change width",
                 mutest_float_value (check.size.width),
                 mutest_to_be_close_to, r.size.width, 0.0001,
                 NULL);
  mutest_expect ("expanding vertically outside the rectangle to change the height",
                 mutest_float_value (check.size.height),
                 mutest_to_be_close_to, p.y, 0.0001,
                 NULL);
  mutest_expect ("expanded rectangle contains original rectangle",
                 mutest_bool_value (graphene_rect_contains_rect (&check, &r)),
                 mutest_to_be_true,
                 NULL);
}

static void
rect_interpolate (mutest_spec_t *spec)
{
  graphene_rect_t a = GRAPHENE_RECT_INIT ( 0.f, 0.f, 10.f, 10.f);
  graphene_rect_t b = GRAPHENE_RECT_INIT (10.f, 5.f, 30.f, 20.f);
  graphene_rect_t c;
  graphene_rect_t res;

  graphene_rect_interpolate (&a, &b, 0.0, &res);
  mutest_expect ("lerp(0) to produce the initial rectangle",
                 mutest_bool_value (graphene_rect_equal (&a, &res)),
                 mutest_to_be_true,
                 NULL);

  graphene_rect_interpolate (&a, &b, 1.0, &res);
  mutest_expect ("lerp(1) to produce the final rectangle",
                 mutest_bool_value (graphene_rect_equal (&b, &res)),
                 mutest_to_be_true,
                 NULL);

  c.origin.x = a.origin.x + (b.origin.x - a.origin.x) * 0.25f;
  c.origin.y = a.origin.y + (b.origin.y - a.origin.y) * 0.25f;
  c.size.width = a.size.width + (b.size.width - a.size.width) * 0.25f;
  c.size.height = a.size.height + (b.size.height - a.size.height) * 0.25f;
  graphene_rect_interpolate (&a, &b, 0.25, &res);
  mutest_expect ("lerp(0.25) to produce the expect rectangle",
                 mutest_bool_value (graphene_rect_equal (&c, &res)),
                 mutest_to_be_true,
                 NULL);
}

static void
rect_suite (mutest_suite_t *suite)
{
  mutest_it ("initialization", rect_init);
  mutest_it ("normalization", rect_normalize);
  mutest_it ("equality", rect_equal);
  mutest_it ("contains points", rect_contains_point);
  mutest_it ("contains rects", rect_contains_rect);
  mutest_it ("intersections", rect_intersect);
  mutest_it ("unions", rect_union);
  mutest_it ("offsets", rect_offset);
  mutest_it ("insets", rect_inset);
  mutest_it ("rounds to nearest integer", rect_round_to_pixel);
  mutest_it ("expands", rect_expand);
  mutest_it ("interpolates", rect_interpolate);
}

MUTEST_MAIN (
  mutest_describe ("graphene_rect_t", rect_suite);
)
