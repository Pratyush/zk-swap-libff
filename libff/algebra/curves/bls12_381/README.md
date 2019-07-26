# Curve BLS12_381 
Zcash pairing-friendly curve (embedding degree `k=12`).

### Progress:
- [x] BLS12_377_g1.cpp  
- [x] BLS12_377_g1.hpp  
- [x] BLS12_377_g2.cpp  
- [x] BLS12_377_g2.hpp  
- [x] BLS12_377_init.cpp  
- [x] BLS12_377_init.hpp  
- [x] BLS12_377_pairing.cpp  
- [x] BLS12_377_pairing.hpp  
- [x] BLS12_377_pp.cpp  
- [x] BLS12_377_pp.hpp

## TODO:
In `BLS12_381_init.hpp`:

* fill in `wnaf_window_table` for G1 and G2 

* fill in `fixed_base_exp_window_table` in G1 and G2

## Tests:
- [x] algebra_field_test
- [x] algebra_group_test
- [x] algebra_bilinearity_test
