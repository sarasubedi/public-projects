 public class FruitEntity extends Entity {
	
	private Game game;
	
	FruitEntity (Game g, String r, int newX, int newY) {
		super(r, newX, newY);  // calls the constructor in Entity
	    game = g; // the game in which FruitEntity exists
	} // constructor
	
	public void collidedWith(Entity other) {
	     if (other instanceof ShipEntity) {
	    	// if fruit touches ship remove it
	    	// and add a point
	        game.removeEntity(this);
	        game.fruitCount++;
	        
	     } // if
	     
	   } // collidedWith

} // FruitEntity