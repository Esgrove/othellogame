use std::collections::BTreeSet;

use shadow_rs::ShadowBuilder;

// Make compile-time build information available.
// https://github.com/baoyachi/shadow-rs

fn main() {
    let deny = BTreeSet::from([shadow_rs::CARGO_METADATA, shadow_rs::CARGO_TREE]);
    ShadowBuilder::builder()
        .deny_const(deny)
        .build()
        .expect("Shadow build failed");
}
