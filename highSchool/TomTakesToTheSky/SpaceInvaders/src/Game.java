/****************************************************************************
* Name:        Object Oriented Address Book
* Author:      Sara Subedi & Becca Pettigrew
* Date:        Oct. 2, 2020
* Purpose:     An object oriented java program that stores
*             addresses like an address book and performs numerous
*             functions on them. 
*****************************************************************************/
import javax.imageio.ImageIO;
import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.awt.geom.Rectangle2D;
import java.awt.image.*;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.awt.image.BufferedImage;  

public class Game extends Canvas {

		protected int gr = 5;// gravity
	
      	private BufferStrategy strategy;   // take advantage of accelerated graphics
      	
        private boolean waitingForKeyPress = true;  // true if game held up until
        private boolean gameRunning = true; // determine if game loop is running
        private boolean logicRequiredThisLoop = false; // true if logic needs to be applied
          
        // a key is pressed
        private boolean firstRun = false;
        private boolean leftPressed = false;  // true if left arrow key currently pressed
        private boolean rightPressed = false; // true if right arrow key currently pressed
        private boolean firePressed = false; // true if firing
        private boolean upPressed = false;
        private boolean downPressed = false;
        private boolean enterPressed = false;
        private boolean iPressed = false;
        private boolean pPressed = false;

     
        private ArrayList entities = new ArrayList(); // list of entities in game                                             
        private ArrayList removeEntities = new ArrayList(); // remove entities                                         

        private Entity ship;  // the ship
        protected String shipState = "";
        protected String shipDirection = "L";
        
        private ArrayList pe = new ArrayList(); // list platform entities
        private Entity platform;
        private int randX = 0;
        private int randY = 0;
        
        private ArrayList bg = new ArrayList(); // list of background entities
        private Entity bgImage; // first background image
        private Entity bgImage2; // tiled background image
        private Entity instructions;
        private boolean instVisible = false;
        private Entity start;
        private Entity startScreen;
        private Entity endScreen;
        private Entity pointer;
        private boolean pointerLoc = true;
       
        private Entity alien;
        
        private ArrayList fruitList = new ArrayList(); // list platform entities
        private Entity fruit;
        protected int fruitCount = 0;
        private Entity holder;
        
        protected int lifeCounter = 0;
        protected Entity heart;
        protected Entity heart2;
        protected Entity heart3;
        
        protected Entity heart4;
        protected Entity heart5;
        protected Entity heart6;
        protected long lastTouch = 0;
        protected long immuneInterval = 2000;

        private double moveSpeed = 90; // hor. vel. of ship (px/s)
        private long lastFire = 0; // time last shot fired
        private long firingInterval = 2000; // interval between shots (ms)

        public String message = ""; // message to display while waiting

    	/*
    	 * Construct our game and set it running.
    	 */
    	public Game() {
    		// create a frame to contain game
    		//JFrame container = new JFrame("Commodore 64 Space Invaders");
    		JFrame container = new JFrame("Invaders of Space");
    		
    		// get hold the content of the frame
    		JPanel panel = (JPanel) container.getContentPane();
 
  	
    		// set up the resolution of the game
    		panel.setPreferredSize(new Dimension(800,600));
    		panel.setLayout(null);
    		
    
    		// set up canvas size (this) and add to frame
    		setBounds(0,0,800,600);
    		panel.add(this);
    

    		// Tell AWT not to bother repainting canvas since that will
            // be done using graphics acceleration
    		setIgnoreRepaint(true);
    
    		// make the window visible
    		container.pack();
    		container.setResizable(false);
    		container.setVisible(true);
    
    
            // if user closes window, shutdown game and jre
    		container.addWindowListener(new WindowAdapter() {
    			public void windowClosing(WindowEvent e) {
    				System.exit(0);
    			} // windowClosing
    		});
    
    		// add key listener to this canvas
    		addKeyListener(new KeyInputHandler());
    
    		// request focus so key events are handled by this canvas
    		requestFocus();

    		// create buffer strategy to take advantage of accelerated graphics
    		createBufferStrategy(2);
    		strategy = getBufferStrategy();
    
    		// initialize entities
    		initEntities();
    
    		
    		
    		/*while (true) {
          	  if (instVisible) {
              	  entities.add(startScreen);
              	  entities.remove(instructions);
                } else if (!instVisible) {
              	  entities.remove(startScreen);
              	  entities.add(instructions);
                }
          	  
          	  if (upPressed) {
          	  	break;
          	  }
    		}  // while*/
    		
    		// start the game
    		
    		gameLoop();
    		
        } // constructor
    
    
        /* initEntities
         * input: none
         * output: none
         * purpose: Initialise the starting state of the ship and alien entities.
         *          Each entity will be added to the array of entities in the game.
    	 */
    	
    
    	
    	
    	private void initEntities() {

        	 for(int i = 0; i < 3; i++) {
        		 randX = 1+(int)(Math.random()*(135 - 5 + 1) + 5);
        		 randY = i*200;
        		 randY -= 450;
        		 platform = new BushEntity(this, "sprites/bush.png", randX, randY + (i * 50));
                 pe.add(platform);
                 entities.add(platform);
                 
                 //createFruit(platform);
        	 }//for 
        	 
        	 for(int i = 0; i < 3; i++) {
        		 randX = (int)(Math.random()*(695 - 535 + 1) + 535);
        		 randY = i*200;
        	
        		 randY -= 450;
        		 platform = new BushEntity(this, "sprites/bush.png", randX, randY - (i * 50));
                 pe.add(platform);
                 entities.add(platform);
                
                // createFruit(platform);
        	 }//for 
        	 
        	 for(int i = 0; i < 3; i++) {
        		 randX = 1+(int)(Math.random()*(439 - 395 + 1) + 395);
        		 randY = -i*200;
        		 randY -= 425;
        		 platform = new BranchREntity(this, "sprites/branchR.png", randX, randY + (i * 50) - 100);
        		 pe.add(platform);
                 entities.add(platform);
                 
                // createFruit(platform);
        	 }//for 
        	 
        	 for(int i = 0; i < 3; i++) {
        		 randX = 1+(int)(Math.random()*(335 - 215 + 1) + 215);
        		 randY = -i*200;
        		
        		 randY -= 425;
        		 platform = new BranchLEntity(this, "sprites/branchL.png", randX, randY - (i * 65) - 80);
                 pe.add(platform);
                 entities.add(platform);
                 
                // createFruit(platform);
                 
                 
        	 }//for 
      

            bgImage = new BgEntity(this, "sprites/bg.png", 0, 0);
            bg.add(bgImage);  
            bgImage2 = new BgEntity(this, "sprites/bg.png", 0, -800);
            bg.add(bgImage2); 
    		
            
            
          
        	alien = new EnemyEntity(this, "sprites/bird.png", 50, -300);
          	entities.add(alien);
          	
          	
          	heart = new HeartEntity(this, "sprites/life.png", -20, -25);
          	entities.add(heart);
          	
          	heart2 = new HeartEntity(this, "sprites/life.png", 15, -25);
          	entities.add(heart2);
          	
          	heart3 = new HeartEntity(this, "sprites/life.png", 50, -25);
          	entities.add(heart3);
          	
          	heart4 = new HeartEntity(this, "sprites/0.png", 735, -25);
          	entities.add(heart4);
         	heart5 = new HeartEntity(this, "sprites/0.png", 710, -25);
          	entities.add(heart5);
         	heart6 = new HeartEntity(this, "sprites/0.png",685, -25);
          	entities.add(heart6);
          	
        	
          	start = new BgEntity(this, "sprites/start.png", 0, -200);
           	entities.add(start);
           	
            shipState = "";
   	        lifeCounter = 0;
   	        shipDirection = "L";
   	        fruitCount = 0;
   	        
            ship = new ShipEntity(this, "sprites/ship" + shipDirection + shipState + ".png", 425, 499);
            entities.add(ship);
            
            instructions = new BgEntity(this, "sprites/instructions.png", 0, -200);
           	
           	
            startScreen = new BgEntity(this, "sprites/startScreen.png", 0, -200);
           	entities.add(startScreen);
           	
           	
           	
           //	pointer = new HeartEntity(this, "sprites/basket.png", 520, 335);
          // 	entities.add(pointer);
           	
           	
    	} // initEntities

    	
    	public String randFruit() {
    		String fruit = "";
    		int rand = (int)(Math.random()*(3-1+1) + 1);
    		
    		switch(rand) {
    			case 1: 
    				fruit = "sprites/mango.png";
    				break;
    			case 2: 
    				fruit = "sprites/cherry.png";
    				break;
    			default:
    				fruit = "sprites/orange.png";
    				break;
    		}
    		
    		
    		return fruit;
    	}
    	
       	public boolean isFruit() {
    		boolean fruit = false;
    		int rand = (int)(Math.random()*(3-1+1) + 1);
    		
    		switch(rand) {
    			case 1: 
    				fruit = true;
    				break;
    			case 2: 
    				fruit = false;
    				break;
    			default:
    				fruit = false;
    				break;
    		}
    		
    		
    		return fruit;
    	}
       	
       	
       	public void createFruit(Entity e){
       		
       		System.out.println(e.x + " " + (int)e.x);
	       	 if(isFruit()) {
	        	 fruit = new FruitEntity(this, randFruit(), (int)e.x + 40,(int) e.y - 20);
	        	 fruitList.add(fruit);
	        	 pe.add(fruit);
	             entities.add(fruit);
	         }
       		
       	}
    	
    	
        /* Notification from a game entity that the logic of the game
         * should be run at the next opportunity 
         */
         public void updateLogic() {
           logicRequiredThisLoop = true;
         } // updateLogic

         /* Remove an entity from the game.  It will no longer be
          * moved or drawn.
          */
         public void removeEntity(Entity entity) {
           removeEntities.add(entity);
         } // removeEntity

         /* Notification that the player has died.
          */
         public void notifyDeath() {
           message = "Well that was an utter failure!  Try again? Your final score was "
           		+ fruitCount;
           waitingForKeyPress = true;
           endScreen = new BgEntity(this, "sprites/endScreen.png", 0, -100);
           bg.add(endScreen); 
           
           entities.clear();
           pe.clear();
           fruitList.clear();
           entities.removeAll(removeEntities);
           removeEntities.clear();
         } // notifyDeath

   
         

         /* Notification that the play has killed all aliens
          */
         public void notifyWin() {
           message = "I guess this is what they mean when they say shoot for the stars!";
           waitingForKeyPress = true;
         } // notifyWin
         
         public void notifyFruitTouch() {
        	 message = "congrats you touched it!";
        	//waitingForKeyPress = true;
         }


        /* Attempt to fire.*/
        public void tryToFire() {
          // check that we've waited long enough to fire
          if ((System.currentTimeMillis() - lastFire) < firingInterval){
            return;
          } // if

          // otherwise add a shot
          lastFire = System.currentTimeMillis();
          /*ShotEntity shot = new ShotEntity(this, "sprites/shot.gif", 
                            ship.getX() + 10, ship.getY() - 30);
          entities.add(shot);
          */
         ship.setFloor(ship.y);
          ship.y -= 80;
          ship.setFloor(500);
        } // tryToFire

	/*
	 * gameLoop
         * input: none
         * output: none
         * purpose: Main game loop. Runs throughout game play.
         *          Responsible for the following activities:
	 *           - calculates speed of the game loop to update moves
	 *           - moves the game entities
	 *           - draws the screen contents (entities, text)
	 *           - updates game events
	 *           - checks input
	 */
       
        Rectangle r = null;
      
	public void gameLoop() {
          long lastLoopTime = System.currentTimeMillis();
         
          
          
	          while (gameRunning) {
	        	  
	        	  
	        	  if (lifeCounter == 1) {
	        		  heart.changeHearts(lifeCounter);
	        		  
	        	  } else if (lifeCounter == 2) {
	        		  heart2.changeHearts(lifeCounter);
	        		  
	        	  } else if (lifeCounter == 3) {
	        		  heart3.changeHearts(lifeCounter);
	        		 
	        	  } else {
	        		  heart.changeHearts(lifeCounter);
	        		  heart2.changeHearts(lifeCounter);
	        		  heart3.changeHearts(lifeCounter);
	        		 
	        	  }
	        	  
	        	  
	        	 //if over 300px screen moves
	        	//if(ship.y < 300) {
	        		 bgImage.y += gr;
	        		 bgImage2.y += gr;
	        		 
	        		 //ship.floor = 500;
	        		if (!((System.currentTimeMillis() - lastTouch) < immuneInterval)){
		
	        			
		                // otherwise add a shot
	        			 if (ship.y >= 500) {
		        			 lastTouch = System.currentTimeMillis();
		        			// System.out.println(lastTouch);
		        			 if (lifeCounter == 2) {
		        	    		 notifyDeath();
		        	    		 lifeCounter++;
		        	    		 shipState = "";
		        	    	     lifeCounter = 0;
		        	    	     
		        	    	 } else if (lifeCounter == 1) {
		        	    		 shipState = "T";
		        	    		 lifeCounter++;
		        	    	 } else {
		        	    		 shipState = "ST";
		        	    	     lifeCounter++;
		        	    	 }
		        		 }
	                }
	        		 
	        		 ship.changeImage("sprites/ship"+ shipDirection + shipState + ".png");
	        			
	        		
	        		 for(int i = 0; i<fruitList.size(); i++) {
	        			 Entity entity = (Entity) fruitList.get(i);
	 
	        			 //entity.y += gr;
	        			  if(entity.y >= 545){ 	
	        				  //System.out.println("test");
	        				 entities.remove(entity);
	        				 fruitList.remove(entity);

	        			 }
	        			  
	        			  
	        			 
	        		 }//for
	        		 
	        		 
	        		 
	        		 for(int i = 0; i<pe.size(); i++) {
	        			 Entity entity = (Entity) pe.get(i);
	        			
	        			 
	        			 
	        			 entity.y += gr;
	        			  if(entity.y >= 550){ 
	        				 
	        				 entity.y -= 600;
	        				 
	        				
	        				 if (entity instanceof BushEntity) {
	        					 entity.changeImage("sprites/bush.png");
	            				 entity.counter = 0;
	            				 //createFruit(entity);
	        				 } else if (entity instanceof BranchLEntity) {
	        					 entity.changeImage("sprites/branchL.png");
	            				 entity.counter = 0;
	            				 createFruit(entity);
	        				 } else if (entity instanceof BranchREntity) {
	        					 entity.changeImage("sprites/branchR.png");
	            				 entity.counter = 0;
	            				 createFruit(entity);
	        				 }
	        				 
	        			 }
	        			 
	        		 }//for
	        		
	        	 
	        		 
	        			 heart4.changeImage("sprites/" + fruitCount%10 + ".png");
	        			 heart5.changeImage("sprites/" + (int)(fruitCount/10) + ".png");
	        			 
	        			 heart6.changeImage("sprites/" + (int)(fruitCount/100) + ".png");
	        			 
	        	 
	        	 if(bgImage.y == 800) {
	        		 bgImage.y = -800;
	        	 }else if(bgImage2.y == 800) {
	        		 bgImage2.y = -800;
	        	 }
	        	 
	            // calc. time since last update, will be used to calculate
	            // entities movement
	            long delta = System.currentTimeMillis() - lastLoopTime;
	            lastLoopTime = System.currentTimeMillis();
	
	            // get graphics context for the accelerated surface and make it black
	            Graphics g = (Graphics2D) strategy.getDrawGraphics();
	    	       
	           // draw all entities
	            for (int i = 0; i < bg.size(); i++) {
	               Entity entity = (Entity) bg.get(i);
	               entity.draw(g);
	              
	            } // for
	
	            // move each entity
	            if (!waitingForKeyPress) {
	            	entities.remove(startScreen);
	            	entities.remove(pointer);
	            	entities.remove(instructions);
	            	//System.out.println(gr);
	            	
	            	if(start.y == 605) {
	            		entities.remove(start);
	            		gr = 5;
	            		ship.floor = 500;
	            		
	            	} 
	            	
	            	if (start.y <= 375){
	            		//System.out.println("test");
	            		ship.setJumped(true);
	            		gr = 0;
	            		start.y+=5;
	            		ship.y-=5;
	            		leftPressed = false;
	            		rightPressed = false;
	            		firePressed = false;
	            		enterPressed = false;
	            		ship.y-=6;
	            	} else if (start.y >= 375) {
	            		start.y+=5;
	            	}
	            	
	            	
	            	
	              for (int i = 0; i < entities.size(); i++) {
	                Entity entity = (Entity) entities.get(i);
	                entity.move(delta);
	                gr = 5;
	                g.fillRect(0,0,0,0);
	              } // for
	            } // if
	
	            // draw all entities
	            for (int i = 0; i < entities.size(); i++) {
	               Entity entity = (Entity) entities.get(i);
	               entity.draw(g);
	              
	            } // for
	            
	            
	
	            // brute force collisions, compare every entity
	            // against every other entity.  If any collisions
	            // are detected notify both entities that it has
	            // occurred
	           for (int i = 0; i < entities.size(); i++) {
	             for (int j = i + 1; j < entities.size(); j++) {
	                Entity me = (Entity)entities.get(i);
	                Entity him = (Entity)entities.get(j);
	
	                if (me.collidesWith(him)) {
	                  me.collidedWith(him);
	                  him.collidedWith(me);
	                } // if
	             } // inner for
	           } // outer for
	
	           // remove dead entities
	           entities.removeAll(removeEntities);
	           removeEntities.clear();
	
	           // run logic if required
	           if (logicRequiredThisLoop) {
	             for (int i = 0; i < entities.size(); i++) {
	               Entity entity = (Entity) entities.get(i);
	               entity.doLogic();
	             } // for
	             logicRequiredThisLoop = false;
	           } // if
	
	           // if waiting for "any key press", draw message
	           if (waitingForKeyPress) {
	        	   
	        	  
	        	 //  g.drawRect(0,0,800,600); // drawRect(x-position, y-position, width, height)
	              // g.setColor(Color.blue);
	        	   
	             //  g.fillRect(0,0,800,600);
	        	  
	        	 gr = 0;
	             //g.setColor(Color.white);
	             //g.drawString(message, (800 - g.getFontMetrics().stringWidth(message))/2, 250);
	             //g.drawString("Press any key", (800 - g.getFontMetrics().stringWidth("Press any key"))/2, 300);
	             ship.setJumped(true);
	             
	             
	             
	            
	           }  // if
	
	            // clear graphics and flip buffer
	            g.dispose();
	            strategy.show();
	
	            // ship should not move without user input
	            ship.setHorizontalMovement(0);
	
	            // respond to user moving ship
	            if ((leftPressed) && (!rightPressed)) {
	              ship.setHorizontalMovement(-moveSpeed);
	            } else if ((rightPressed) && (!leftPressed)) {
	              ship.setHorizontalMovement(moveSpeed);
	            }
	            
	            //make the ship jump/fall
	            ship.jump(ship);
	            ship.fall(ship);
	         
	
	            // if spacebar pressed, try to fire
	            if (firePressed) {
	              tryToFire();
	            } // if
	
	            // pause
	            try { Thread.sleep(100); } catch (Exception e) {}
	
	          } // while
          
          
          //} // if

	} // gameLoop

	
	

	
        /* startGame
         * input: none
         * output: none
         * purpose: start a fresh game, clear old data
         */
         private void startGame() {
            // clear out any existing entities and initalize a new set
        
            entities.clear();
            initEntities();
            
          
            
            // blank out any keyboard settings that might exist
            leftPressed = false;
            rightPressed = false;
            firePressed = false;
            upPressed = false;
            downPressed = false;
         } // startGame


        /* inner class KeyInputHandler
         * handles keyboard input from the user
         */
	private class KeyInputHandler extends KeyAdapter {
                 
                 private int pressCount = 1;  // the number of key presses since
                                              // waiting for 'any' key press

                /* The following methods are required
                 * for any class that extends the abstract
                 * class KeyAdapter.  They handle keyPressed,
                 * keyReleased and keyTyped events.
                 */
		public void keyPressed(KeyEvent e) {

                  // if waiting for keypress to start game, do nothing
                  if (waitingForKeyPress) {
                    return;
                  } // if
                  
                  // respond to move left, right or fire
                  if (e.getKeyCode() == KeyEvent.VK_LEFT) {
                    leftPressed = true;
                    shipDirection = "L";
                  } // if

                  if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
                    rightPressed = true;
                    shipDirection = "R";
                  } // if
                  

                  if (e.getKeyCode() == KeyEvent.VK_SPACE) {
                    firePressed = true;
                  } // if
                  
                  if (e.getKeyCode() == KeyEvent.VK_UP) {
                      upPressed = true;
                      pointerLoc = true;

                      //pointer.x = 535;
                      //pointer.y = 335;
                      	
                      //pointerLoc = (pointerLoc) ? false : true;
                    } // if
                  
                  if (e.getKeyCode() == KeyEvent.VK_DOWN) {
                      downPressed = true;
                      //pointerLoc = false;

                     //pointer.x = 700;
                     //pointer.y = 500;
                     	
                      //pointerLoc = (pointerLoc) ? false : true;
                    } // if
                  
                  /*if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                      enterPressed = true;
                      //System.out.println(pointerLoc);
                      //if(pointerLoc) {
                      //entities.remove(startScreen);
                      //entities.remove(pointer);
                      }*/
                  
                  if (e.getKeyCode() == 73) {
                      iPressed = true;
                      
                      
                    } // if
                  
                  if (e.getKeyCode() == 80) {
                	  pPressed = true;
                  }
		
	
                    
                  

		} // keyPressed

		public void keyReleased(KeyEvent e) {
                  // if waiting for keypress to start game, do nothing
                  if (enterPressed) {
                	  enterPressed = false;
                    return;
                  } // if
                  
                  // respond to move left, right or fire
                  if (e.getKeyCode() == KeyEvent.VK_LEFT) {
                    leftPressed = false;
                  } // if

                  if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
                    rightPressed = false;
                  } // if
                  
                  if (e.getKeyCode() == KeyEvent.VK_SPACE) {
                    firePressed = false;
                  } // if
                  
                  if (e.getKeyCode() == KeyEvent.VK_UP) {
                      upPressed = false;
                    } // if
                  
                  if (e.getKeyCode() == KeyEvent.VK_DOWN) {
                      downPressed = false;
                    } // if
                  
                  /*if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                      enterPressed = false;
                    } // if*/
                  
                  if (e.getKeyCode() == 73) {
                      iPressed = true;
                    } // if
                  
                  if (e.getKeyCode() == 80) {
                	  pPressed = true;
                  }

		} // keyReleased

 	        public void keyTyped(KeyEvent e) {

                   // if waiting for key press to start game
 	           if (waitingForKeyPress) {
 	        	if(pressCount == 1 && !firstRun) {
 	        		entities.add(instructions);
 	        		entities.remove(startScreen);
 	        		waitingForKeyPress = true;
 	        		iPressed = false;
 	        		firstRun = true;
 	        		//pressCount = 0;
 	        		return;
 	           } else if (pressCount == 2 && firstRun)  {
 	        	  System.out.println("pink dinosaur");
                  waitingForKeyPress = false;
                  pPressed = false;
                  startGame();
                  //pressCount = 0;
 	           }  
 	        	   
 	        	else if (pressCount == 1) {
 	           
                       System.out.println("purple dinosaur");
                       waitingForKeyPress = false;
                       entities.remove(instructions);
                       startGame();
                       pressCount++;
                       pPressed = false;
                     } else {
                       pressCount++;
                     } 
                   } // if waitingForKeyPress

                   // if escape is pressed, end game
                   if (e.getKeyChar() == 27) {
                     System.exit(0);
                   } // if escape pressed

		} // keyTyped

	}// class KeyInputHandler


	/**
	 * Main Program
	 */
	public static void main(String [] args) {
        // instantiate this object
		new Game();
	} // main
} // Game