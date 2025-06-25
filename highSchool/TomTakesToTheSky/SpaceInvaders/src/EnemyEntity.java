/* EnemyEntity.java
 * March 27, 2006
 * Represents one of the Enemies
 */
public class EnemyEntity extends Entity {

  private double moveSpeed = (int)(Math.random() * ((150 - 75) + 1)) + 75; // horizontal speed
  protected int yUpBounds; // highest point of enemy movement
  protected int yDownBounds; // lowest point of enemy movement
  private Game game; // the game in which the enemy exists

  /* construct a new enemy
   * input: game - the game in which the enemy is being created
   *        r - the image representing the enemy
   *        x, y - initial location of enemy
   */
  public EnemyEntity(Game g, String r, int newX, int newY) {
    super(r, newX, newY);  // calls the constructor in Entity
    game = g;
    dx = -moveSpeed;  // start off moving left
    dy = -75;

    yUpBounds = (int)y-50;
    yDownBounds = (int)y+50; 
  } // constructor

  /* move
   * input: delta - time elapsed since last move (ms)
   * purpose: move enemy
   */
  public void move (long delta){
    // if we reach left side of screen and are moving left
    // request logic update
	
	// change direction of movement down
	if(y < yUpBounds) {
		 dy *= -1;
	}
	
	// change direction of movement up
	if(y > yDownBounds) {
		 dy *= -1; 
	}

	// if left side of screen is reached update logic and sprite
    if ((dx < 0) && (x < 100)) {
        changeImage("sprites/birdR.png");
    	y += 100;
        yUpBounds = (int)y-50;
        yDownBounds = (int)y+50;
        game.updateLogic();   // logic deals with moving entities
                            // in other direction and down screen
    } // if
    
    
    if(y > 550) {
    	 y  -= (int)(Math.random() * ((1000 - 700) + 1)) + 700;
    	
         yUpBounds = (int)y-50;
         yDownBounds = (int)y+50;
    }
    
    

    // if we reach right side of screen and are moving right
    // request logic update
    if ((dx > 0) && (x > 650)) {
     y += 100;
     changeImage("sprites/bird.png");
     yUpBounds = (int)y-50;
     yDownBounds = (int)y+50;
      game.updateLogic();
    } // if 
    
   
    
    
    
    
    
    // proceed with normal move
    super.move(delta);
  } // move


  /* doLogic
   * Updates the game logic related to the enemys,
   * ie. move it down the screen and change direction
   */
  public void doLogic() {
    // swap horizontal direction
    dx *= -1;
  } // doLogic


  /* collidedWith
   * input: other - the entity with which the enemy has collided
   * purpose: notification that the enemy has collided
   *          with something
   */
   public void collidedWith(Entity other) {
     // collisions with enemies are handled in ShipEntity
   } // collidedWith
  
} // EnemyEntity class