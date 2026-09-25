// Aplicación de Representaciones de Rotación en 3D
class RotationDemo {
    constructor() {
        this.representationType = 'euler';
        this.eulerAngles = { x: 0, y: 0, z: 0 };
        this.quaternion = { w: 1, x: 0, y: 0, z: 0 };
        this.matrix = new THREE.Matrix4();
        this.isAnimating = false;
        this.showComparison = false;
        this.gimbalLockDemo = false;
        
        this.scene = null;
        this.renderer = null;
        this.camera = null;
        this.cube = null;
        this.cube2 = null;
        this.cube3 = null;
        this.animationFrameId = null;
        
        this.init();
        this.setupEventListeners();
        this.updateUI();
    }
    
    init() {
        const container = document.getElementById('three-container');
        
        // Scene setup
        this.scene = new THREE.Scene();
        this.scene.background = new THREE.Color(0x1a1a2e);
        
        // Camera
        this.camera = new THREE.PerspectiveCamera(75, 800 / 600, 0.1, 1000);
        this.camera.position.set(5, 5, 5);
        this.camera.lookAt(0, 0, 0);
        
        // Renderer
        this.renderer = new THREE.WebGLRenderer({ antialias: true });
        this.renderer.setSize(800, 600);
        this.renderer.shadowMap.enabled = true;
        this.renderer.shadowMap.type = THREE.PCFSoftShadowMap;
        
        container.appendChild(this.renderer.domElement);
        
        // Lighting
        const ambientLight = new THREE.AmbientLight(0x404040, 0.6);
        this.scene.add(ambientLight);
        
        const directionalLight = new THREE.DirectionalLight(0xffffff, 0.8);
        directionalLight.position.set(10, 10, 5);
        directionalLight.castShadow = true;
        this.scene.add(directionalLight);
        
        // Create cubes
        this.createCubes();
        
        // Add coordinate axes
        const axesHelper = new THREE.AxesHelper(2);
        this.scene.add(axesHelper);
        
        // Add ground plane
        const planeGeometry = new THREE.PlaneGeometry(10, 10);
        const planeMaterial = new THREE.MeshLambertMaterial({ 
            color: 0x666666, 
            transparent: true, 
            opacity: 0.3 
        });
        const plane = new THREE.Mesh(planeGeometry, planeMaterial);
        plane.rotation.x = -Math.PI / 2;
        plane.position.y = -2;
        plane.receiveShadow = true;
        this.scene.add(plane);
        
        // Start animation loop
        this.animate();
        
        // Handle window resize
        window.addEventListener('resize', () => this.onWindowResize());
    }
    
    createCubes() {
        const geometry = new THREE.BoxGeometry(1, 1, 1);
        
        // Materials for colored faces (for orientation)
        const materials = [
            new THREE.MeshLambertMaterial({ color: 0xff0000 }), // Right - Red
            new THREE.MeshLambertMaterial({ color: 0x00ff00 }), // Left - Green  
            new THREE.MeshLambertMaterial({ color: 0x0000ff }), // Top - Blue
            new THREE.MeshLambertMaterial({ color: 0xffff00 }), // Bottom - Yellow
            new THREE.MeshLambertMaterial({ color: 0xff00ff }), // Front - Magenta
            new THREE.MeshLambertMaterial({ color: 0x00ffff })  // Back - Cyan
        ];
        
        // Main cube
        this.cube = new THREE.Mesh(geometry, materials);
        this.cube.castShadow = true;
        this.scene.add(this.cube);
        
        // Comparison cubes
        this.cube2 = new THREE.Mesh(geometry, materials.map(m => m.clone()));
        this.cube2.position.x = -3;
        this.cube2.castShadow = true;
        this.cube2.visible = false;
        this.scene.add(this.cube2);
        
        this.cube3 = new THREE.Mesh(geometry, materials.map(m => m.clone()));
        this.cube3.position.x = 3;
        this.cube3.castShadow = true;
        this.cube3.visible = false;
        this.scene.add(this.cube3);
    }
    
    setupEventListeners() {
        // Representation type buttons
        document.getElementById('euler-btn').addEventListener('click', () => {
            this.setRepresentationType('euler');
        });
        
        document.getElementById('quaternion-btn').addEventListener('click', () => {
            this.setRepresentationType('quaternion');
        });
        
        document.getElementById('matrix-btn').addEventListener('click', () => {
            this.setRepresentationType('matrix');
        });
        
        // Action buttons
        document.getElementById('gimbal-lock-btn').addEventListener('click', () => {
            this.demonstrateGimbalLock();
        });
        
        document.getElementById('interpolation-btn').addEventListener('click', () => {
            this.demonstrateInterpolation();
        });
        
        document.getElementById('comparison-btn').addEventListener('click', () => {
            this.toggleComparison();
        });
        
        document.getElementById('reset-btn').addEventListener('click', () => {
            this.resetRotation();
        });
        
        // Euler angle sliders
        document.getElementById('euler-x').addEventListener('input', (e) => {
            this.eulerAngles.x = parseFloat(e.target.value);
            this.updateRotation();
            this.updateUI();
        });
        
        document.getElementById('euler-y').addEventListener('input', (e) => {
            this.eulerAngles.y = parseFloat(e.target.value);
            this.updateRotation();
            this.updateUI();
        });
        
        document.getElementById('euler-z').addEventListener('input', (e) => {
            this.eulerAngles.z = parseFloat(e.target.value);
            this.updateRotation();
            this.updateUI();
        });
        
        // Quaternion sliders
        document.getElementById('quat-w').addEventListener('input', (e) => {
            this.quaternion.w = parseFloat(e.target.value);
            this.updateRotation();
            this.updateUI();
        });
        
        document.getElementById('quat-x').addEventListener('input', (e) => {
            this.quaternion.x = parseFloat(e.target.value);
            this.updateRotation();
            this.updateUI();
        });
        
        document.getElementById('quat-y').addEventListener('input', (e) => {
            this.quaternion.y = parseFloat(e.target.value);
            this.updateRotation();
            this.updateUI();
        });
        
        document.getElementById('quat-z').addEventListener('input', (e) => {
            this.quaternion.z = parseFloat(e.target.value);
            this.updateRotation();
            this.updateUI();
        });
    }
    
    setRepresentationType(type) {
        this.representationType = type;
        
        // Update button states
        document.querySelectorAll('.representation-btn').forEach(btn => {
            btn.classList.remove('active');
        });
        document.getElementById(`${type}-btn`).classList.add('active');
        
        // Enable/disable sliders
        this.updateSliderStates();
        this.updateRotation();
    }
    
    updateSliderStates() {
        const eulerSliders = ['euler-x', 'euler-y', 'euler-z'];
        const quatSliders = ['quat-w', 'quat-x', 'quat-y', 'quat-z'];
        
        eulerSliders.forEach(id => {
            document.getElementById(id).disabled = this.representationType !== 'euler' || this.isAnimating;
        });
        
        quatSliders.forEach(id => {
            document.getElementById(id).disabled = this.representationType !== 'quaternion' || this.isAnimating;
        });
    }
    
    updateRotation() {
        if (!this.cube) return;
        
        if (this.representationType === 'euler') {
            this.cube.rotation.set(
                THREE.MathUtils.degToRad(this.eulerAngles.x),
                THREE.MathUtils.degToRad(this.eulerAngles.y),
                THREE.MathUtils.degToRad(this.eulerAngles.z)
            );
            
            // Update quaternion and matrix from euler
            const newQuaternion = new THREE.Quaternion().setFromEuler(this.cube.rotation);
            this.quaternion = {
                w: newQuaternion.w,
                x: newQuaternion.x,
                y: newQuaternion.y,
                z: newQuaternion.z
            };
            
            this.matrix = new THREE.Matrix4().makeRotationFromQuaternion(newQuaternion);
            
        } else if (this.representationType === 'quaternion') {
            const q = new THREE.Quaternion(
                this.quaternion.x, 
                this.quaternion.y, 
                this.quaternion.z, 
                this.quaternion.w
            ).normalize();
            
            this.cube.quaternion.copy(q);
            
            // Update euler angles and matrix
            const euler = new THREE.Euler().setFromQuaternion(q);
            this.eulerAngles = {
                x: THREE.MathUtils.radToDeg(euler.x),
                y: THREE.MathUtils.radToDeg(euler.y),
                z: THREE.MathUtils.radToDeg(euler.z)
            };
            
            this.matrix = new THREE.Matrix4().makeRotationFromQuaternion(q);
            
        } else if (this.representationType === 'matrix') {
            this.cube.matrix.copy(this.matrix);
            this.cube.matrixAutoUpdate = false;
            
            // Update quaternion and euler from matrix
            const newQuaternion = new THREE.Quaternion().setFromRotationMatrix(this.matrix);
            this.quaternion = {
                w: newQuaternion.w,
                x: newQuaternion.x,
                y: newQuaternion.y,
                z: newQuaternion.z
            };
            
            const euler = new THREE.Euler().setFromRotationMatrix(this.matrix);
            this.eulerAngles = {
                x: THREE.MathUtils.radToDeg(euler.x),
                y: THREE.MathUtils.radToDeg(euler.y),
                z: THREE.MathUtils.radToDeg(euler.z)
            };
        }
        
        // Update comparison cubes if visible
        if (this.showComparison) {
            // Cube 2: Euler
            this.cube2.rotation.set(
                THREE.MathUtils.degToRad(this.eulerAngles.x),
                THREE.MathUtils.degToRad(this.eulerAngles.y),
                THREE.MathUtils.degToRad(this.eulerAngles.z)
            );
            
            // Cube 3: Quaternion
            const q = new THREE.Quaternion(
                this.quaternion.x, 
                this.quaternion.y, 
                this.quaternion.z, 
                this.quaternion.w
            ).normalize();
            this.cube3.quaternion.copy(q);
        }
    }
    
    updateUI() {
        // Update Euler angle displays
        document.getElementById('euler-x-value').textContent = this.eulerAngles.x.toFixed(1) + '°';
        document.getElementById('euler-y-value').textContent = this.eulerAngles.y.toFixed(1) + '°';
        document.getElementById('euler-z-value').textContent = this.eulerAngles.z.toFixed(1) + '°';
        
        // Update slider values
        document.getElementById('euler-x').value = this.eulerAngles.x;
        document.getElementById('euler-y').value = this.eulerAngles.y;
        document.getElementById('euler-z').value = this.eulerAngles.z;
        
        // Update quaternion displays
        document.getElementById('quat-w-value').textContent = this.quaternion.w.toFixed(3);
        document.getElementById('quat-x-value').textContent = this.quaternion.x.toFixed(3);
        document.getElementById('quat-y-value').textContent = this.quaternion.y.toFixed(3);
        document.getElementById('quat-z-value').textContent = this.quaternion.z.toFixed(3);
        
        // Update quaternion sliders
        document.getElementById('quat-w').value = this.quaternion.w;
        document.getElementById('quat-x').value = this.quaternion.x;
        document.getElementById('quat-y').value = this.quaternion.y;
        document.getElementById('quat-z').value = this.quaternion.z;
        
        // Update quaternion magnitude
        const magnitude = Math.sqrt(
            this.quaternion.w**2 + this.quaternion.x**2 + 
            this.quaternion.y**2 + this.quaternion.z**2
        );
        document.getElementById('quat-magnitude').textContent = magnitude.toFixed(3);
        
        // Update matrix display
        this.updateMatrixDisplay();
        
        // Update gimbal lock warning
        const gimbalWarning = document.getElementById('gimbal-warning');
        if (Math.abs(this.eulerAngles.y) > 85) {
            gimbalWarning.classList.remove('hidden');
        } else {
            gimbalWarning.classList.add('hidden');
        }
        
        // Update slider states
        this.updateSliderStates();
    }
    
    updateMatrixDisplay() {
        const matrixDisplay = document.getElementById('matrix-display');
        matrixDisplay.innerHTML = '';
        
        const elements = this.matrix.elements;
        const matrixIndices = [0, 4, 8, 1, 5, 9, 2, 6, 10]; // Corrected for Three.js column-major order
        
        matrixIndices.forEach(index => {
            const element = document.createElement('div');
            element.className = 'matrix-element';
            element.textContent = elements[index].toFixed(2);
            matrixDisplay.appendChild(element);
        });
        
        // Update determinant
        document.getElementById('matrix-determinant').textContent = this.matrix.determinant().toFixed(3);
    }
    
    demonstrateGimbalLock() {
        if (this.gimbalLockDemo || this.isAnimating) return;
        
        this.gimbalLockDemo = true;
        this.setRepresentationType('euler');
        
        const startAngles = { ...this.eulerAngles };
        const targetAngles = { x: 0, y: 90, z: 0 };
        
        const button = document.getElementById('gimbal-lock-btn');
        button.textContent = 'Demostrando...';
        button.disabled = true;
        
        document.getElementById('demo-info').classList.remove('hidden');
        
        let progress = 0;
        const duration = 2000;
        const startTime = Date.now();
        
        const animate = () => {
            const elapsed = Date.now() - startTime;
            progress = Math.min(elapsed / duration, 1);
            
            this.eulerAngles = {
                x: startAngles.x + (targetAngles.x - startAngles.x) * progress,
                y: startAngles.y + (targetAngles.y - startAngles.y) * progress,
                z: startAngles.z + (targetAngles.z - startAngles.z) * progress
            };
            
            this.updateRotation();
            this.updateUI();
            
            if (progress < 1) {
                requestAnimationFrame(animate);
            } else {
                // Now show the problem by trying to rotate X and Z
                setTimeout(() => {
                    this.eulerAngles.x = 45;
                    this.updateRotation();
                    this.updateUI();
                }, 1000);
                
                setTimeout(() => {
                    this.eulerAngles.z = 45;
                    this.updateRotation();
                    this.updateUI();
                }, 2000);
            }
        };
        
        animate();
        
        setTimeout(() => {
            this.gimbalLockDemo = false;
            button.textContent = 'Demo Gimbal Lock';
            button.disabled = false;
            document.getElementById('demo-info').classList.add('hidden');
        }, 5000);
    }
    
    demonstrateInterpolation() {
        if (this.isAnimating || this.gimbalLockDemo) return;
        
        this.isAnimating = true;
        this.setRepresentationType('quaternion');
        
        const button = document.getElementById('interpolation-btn');
        button.textContent = 'Interpolando...';
        button.disabled = true;
        
        const startQuaternion = new THREE.Quaternion(
            this.quaternion.x, this.quaternion.y, this.quaternion.z, this.quaternion.w
        );
        const endQuaternion = new THREE.Quaternion().setFromEuler(
            new THREE.Euler(Math.PI / 4, Math.PI / 3, Math.PI / 6)
        );
        
        let progress = 0;
        const duration = 3000;
        const startTime = Date.now();
        
        const animate = () => {
            const elapsed = Date.now() - startTime;
            progress = (elapsed / duration) % 1;
            
            // SLERP interpolation
            const interpolatedQuaternion = new THREE.Quaternion()
                .slerpQuaternions(startQuaternion, endQuaternion, progress);
            
            this.quaternion = {
                w: interpolatedQuaternion.w,
                x: interpolatedQuaternion.x,
                y: interpolatedQuaternion.y,
                z: interpolatedQuaternion.z
            };
            
            this.updateRotation();
            this.updateUI();
            
            if (elapsed < duration * 3) { // Repeat 3 times
                requestAnimationFrame(animate);
            } else {
                this.isAnimating = false;
                button.textContent = 'Demo Interpolación';
                button.disabled = false;
            }
        };
        
        animate();
    }
    
    toggleComparison() {
        this.showComparison = !this.showComparison;
        
        this.cube2.visible = this.showComparison;
        this.cube3.visible = this.showComparison;
        
        const button = document.getElementById('comparison-btn');
        const labels = document.getElementById('comparison-labels');
        
        if (this.showComparison) {
            button.textContent = 'Ocultar Comparación';
            labels.classList.remove('hidden');
            this.cube.position.x = 0;
        } else {
            button.textContent = 'Mostrar Comparación';
            labels.classList.add('hidden');
            this.cube.position.x = 0;
        }
        
        this.updateRotation();
    }
    
    resetRotation() {
        this.eulerAngles = { x: 0, y: 0, z: 0 };
        this.quaternion = { w: 1, x: 0, y: 0, z: 0 };
        this.matrix = new THREE.Matrix4();
        
        if (this.cube) {
            this.cube.matrixAutoUpdate = true;
        }
        
        this.updateRotation();
        this.updateUI();
    }
    
    animate() {
        this.animationFrameId = requestAnimationFrame(() => this.animate());
        this.renderer.render(this.scene, this.camera);
    }
    
    onWindowResize() {
        const container = document.getElementById('three-container');
        const width = Math.min(800, container.clientWidth);
        const height = 600;
        
        this.camera.aspect = width / height;
        this.camera.updateProjectionMatrix();
        this.renderer.setSize(width, height);
    }
    
    dispose() {
        if (this.animationFrameId) {
            cancelAnimationFrame(this.animationFrameId);
        }
        if (this.renderer) {
            this.renderer.dispose();
        }
    }
}

// Initialize the application when the page loads
document.addEventListener('DOMContentLoaded', () => {
    new RotationDemo();
});

// Handle page unload
window.addEventListener('beforeunload', () => {
    if (window.rotationDemo) {
        window.rotationDemo.dispose();
    }
});