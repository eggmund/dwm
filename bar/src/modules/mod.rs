mod idena;
pub use idena::*;

use crate::config;
// Module is a trait for making the update systems modular, so it is easier to add modules
pub trait Module {
    async fn update(&mut self) -> Result<(), Box<dyn std::error::Error>>;
    fn needs_update(&self, update_counter: u64) -> bool {
        true
    }
    fn output(&self) -> String; // Get module output
}
